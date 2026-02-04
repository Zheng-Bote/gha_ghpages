/**
 * SPDX-FileComment: Plugin Interface Definition
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file interfaces.hpp
 * @brief Defines the IPlugin interface for extending the SSG.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#include "model/context.hpp"
#include <md4c.h>
#include <string>
#include <expected>
#include <memory>
#include <iostream>

namespace ssg::core {

class IPluginHost {
public:
    virtual ~IPluginHost() = default;

    // Dispatch methods for Markdown parsing events
    virtual bool dispatch_md_enter(MD_BLOCKTYPE type, void* detail, model::PageContext& ctx, std::ostream& out) = 0;
    virtual bool dispatch_md_leave(MD_BLOCKTYPE type, void* detail, model::PageContext& ctx, std::ostream& out) = 0;
    virtual bool dispatch_md_text(MD_TEXTTYPE type, const char* text, MD_SIZE size, model::PageContext& ctx, std::ostream& out) = 0;
};

/**
 * @brief Interface for a renderer that handles specific file types.
 */
class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    /**
     * @brief Renders the content to HTML.
     * @param ctx The page context.
     * @return std::expected<std::string, std::string> Resulting HTML or error message.
     */
    virtual std::expected<std::string, std::string> render(model::PageContext& ctx) = 0;
};

/**
 * @brief Main Plugin Interface.
 */
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    /**
     * @brief Returns the unique name of the plugin.
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief Returns the version of the plugin.
     */
    virtual std::string version() const { return "1.0.0"; }

    /**
     * @brief Returns a description of the plugin.
     */
    virtual std::string description() const { return ""; }

    // --- Lifecycle Hooks ---

    /**
     * @brief Called when the application starts, after config is loaded.
     */
    virtual void on_init(model::SiteContext& ctx) {}
    
    /**
     * @brief Called after all pages are generated.
     */
    virtual void on_post_build(model::SiteContext& ctx) {}

    // --- Parsing / Rendering Capability ---

    /**
     * @brief Checks if this plugin can provide a renderer for the given file extension.
     * @param extension The file extension (e.g., ".md").
     * @return std::unique_ptr<IRenderer> A renderer instance if supported, otherwise nullptr.
     */
    virtual std::unique_ptr<IRenderer> create_renderer(const std::string& extension) { return nullptr; }

    // --- Processing Hooks ---

    /**
     * @brief Called before the renderer processes the page.
     * Useful for modifying raw content or metadata.
     */
    virtual void on_before_render(model::PageContext& ctx) {}
    
    /**
     * @brief Called after the renderer has produced HTML.
     * Useful for post-processing HTML (e.g., TOC injection).
     */
    virtual void on_after_render(model::PageContext& ctx) {}

    // --- MD4C Low-Level Hooks (Optional) ---
    // These are specific to Markdown rendering. Plugins can override these to intercept parsing events.
    // Returning true indicates the event was handled and standard processing should stop/change.
    
    virtual bool on_md_block_enter(MD_BLOCKTYPE type, void* detail, model::PageContext& ctx, std::ostream& out) { return false; }
    virtual bool on_md_block_leave(MD_BLOCKTYPE type, void* detail, model::PageContext& ctx, std::ostream& out) { return false; }
    virtual bool on_md_text(MD_TEXTTYPE type, const char* text, MD_SIZE size, model::PageContext& ctx, std::ostream& out) { return false; }
};

// Factory types for dynamic loading
using CreatePluginFunc = IPlugin* (*)(IPluginHost* host);
using DestroyPluginFunc = void (*)(IPlugin* plugin);

} // namespace ssg::core
