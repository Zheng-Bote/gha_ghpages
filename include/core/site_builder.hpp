/**
 * SPDX-FileComment: Site Builder Header
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file site_builder.hpp
 * @brief Main logic for building the static site.
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
#include "core/plugin_manager.hpp"
#include <filesystem>
#include <inja.hpp>
#include <vector>

namespace ssg::core {

namespace fs = std::filesystem;

struct DirNode {
    fs::path relative_path;
    std::string dir_name;
    std::vector<fs::path> files;
    std::vector<DirNode> subdirs;
};

class SiteBuilder {
    model::SiteContext site_ctx;
    PluginManager plugin_manager;
    inja::Environment inja_env;
    DirNode root_node; // Cache the file structure

public:
    explicit SiteBuilder(const std::string& config_path);

    void set_source_root(const fs::path& path) { site_ctx.source_root = path; }

    void register_plugins();
    void build();

private:
    void load_config(const std::string& config_path);
    
    // Phase 1: Scan
    DirNode scan_directory(const fs::path& current_path);
    bool is_supported(const fs::path& p);
    
    // Phase 2: Render
    void process_node(const DirNode& node);
    void process_file(const fs::path& file_path, const DirNode& current_node);
    
    // Helpers
    std::string generate_nav_html(const DirNode& node, const std::string& url_prefix, const fs::path& active_file, bool is_root);
};

} // namespace ssg::core
