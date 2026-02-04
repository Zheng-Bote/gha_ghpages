/**
 * SPDX-FileComment: Plugin Manager Definition
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file plugin_manager.hpp
 * @brief Manages loading and dispatching events to plugins.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#pragma once

#include "core/interfaces.hpp"
#include <vector>
#include <memory>
#include <print>
#include <format>
#include <algorithm>
#include <filesystem>
#include <dlfcn.h>

namespace ssg::core {

class PluginManager : public IPluginHost {
    struct LibraryHandle {
        void* handle;
        std::string path;

        LibraryHandle(void* h, std::string p) : handle(h), path(std::move(p)) {}
        ~LibraryHandle() {
            if (handle) {
                dlclose(handle);
            }
        }
        // Non-copyable
        LibraryHandle(const LibraryHandle&) = delete;
        LibraryHandle& operator=(const LibraryHandle&) = delete;
        // Movable
        LibraryHandle(LibraryHandle&& other) noexcept : handle(other.handle), path(std::move(other.path)) {
            other.handle = nullptr;
        }
        LibraryHandle& operator=(LibraryHandle&& other) noexcept {
            if (this != &other) {
                if (handle) dlclose(handle);
                handle = other.handle;
                path = std::move(other.path);
                other.handle = nullptr;
            }
            return *this;
        }
    };

    std::vector<LibraryHandle> libraries;
    std::vector<std::shared_ptr<IPlugin>> plugins; // Changed to shared_ptr to handle custom deleters

public:
    /**
     * @brief Loads a plugin from a shared object file.
     * @param path Path to the .so file.
     */
    void load_plugin_from_file(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            std::println(stderr, "Plugin file not found: {}", path.string());
            return;
        }

        void* handle = dlopen(path.c_str(), RTLD_NOW);
        if (!handle) {
            std::println(stderr, "Failed to load plugin {}: {}", path.string(), dlerror());
            return;
        }

        // Reset errors
        dlerror();

        // Load create symbol
        auto create_func = reinterpret_cast<CreatePluginFunc>(dlsym(handle, "create_plugin"));
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            std::println(stderr, "Cannot load symbol 'create_plugin': {}", dlsym_error);
            dlclose(handle);
            return;
        }

        // Create plugin instance
        IPlugin* raw_plugin = create_func(this);
        if (!raw_plugin) {
            std::println(stderr, "Failed to create plugin instance from: {}", path.string());
            dlclose(handle);
            return;
        }

        // Store library handle and plugin
        libraries.emplace_back(handle, path.string());
        
        auto destroy_func = reinterpret_cast<DestroyPluginFunc>(dlsym(handle, "destroy_plugin"));
        if (destroy_func) {
             plugins.push_back(std::shared_ptr<IPlugin>(raw_plugin, destroy_func));
        } else {
             plugins.push_back(std::shared_ptr<IPlugin>(raw_plugin));
        }

        std::println("Loaded Plugin: {} v{} - {}", raw_plugin->name(), raw_plugin->version(), raw_plugin->description());
    }

    /**
     * @brief Dispatches a generic event to all plugins.
     * @param func Lambda to call on each plugin.
     */
    template<typename Func>
    void dispatch(Func func) {
        for (auto& plugin : plugins) {
            try {
                func(*plugin);
            } catch (const std::exception& e) {
                std::println(stderr, "Error in Plugin '{}': {}", plugin->name(), e.what());
            }
        }
    }

    /**
     * @brief Finds a suitable renderer for the given extension.
     * Queries all plugins; returns the first one that claims support.
     */
    std::unique_ptr<IRenderer> get_renderer(const std::string& extension) {
        for (auto& plugin : plugins) {
            auto renderer = plugin->create_renderer(extension);
            if (renderer) {
                return renderer;
            }
        }
        return nullptr;
    }

    // --- Specialized Dispatchers for MD4C (IPluginHost Implementation) ---

    bool dispatch_md_enter(MD_BLOCKTYPE type, void* detail, model::PageContext& ctx, std::ostream& out) override {
        for (auto& plugin : plugins) {
            if (plugin->on_md_block_enter(type, detail, ctx, out)) return true;
        }
        return false;
    }
    
    bool dispatch_md_leave(MD_BLOCKTYPE type, void* detail, model::PageContext& ctx, std::ostream& out) override {
        for (auto& plugin : plugins) {
            if (plugin->on_md_block_leave(type, detail, ctx, out)) return true;
        }
        return false;
    }

    bool dispatch_md_text(MD_TEXTTYPE type, const char* text, MD_SIZE size, model::PageContext& ctx, std::ostream& out) override {
        for (auto& plugin : plugins) {
            if (plugin->on_md_text(type, text, size, ctx, out)) return true;
        }
        return false;
    }
};

} // namespace ssg::core
