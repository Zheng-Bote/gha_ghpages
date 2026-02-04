/**
 * SPDX-FileComment: Site Builder Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file site_builder.cpp
 * @brief Implementation of the SiteBuilder.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/site_builder.hpp"

#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <inicpp.h>

namespace ssg::core {

// Helper: Read File
std::string read_file(const fs::path& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in) throw std::runtime_error(std::format("Cannot read: {}", path.string()));
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

// Helper: Write File
void write_file(const fs::path& path, std::string_view content) {
    std::ofstream out(path, std::ios::out | std::ios::binary);
    if (!out) throw std::runtime_error(std::format("Cannot write: {}", path.string()));
    out << content;
}

SiteBuilder::SiteBuilder(const std::string& config_path) {
    load_config(config_path);
}

void SiteBuilder::load_config(const std::string& config_path) {
    if (!fs::exists(config_path)) throw std::runtime_error("Config file not found");

    ini::IniFile ini(config_path);

    // Lambda to try finding a key in common sections
    auto get_val = [&](const std::string& key) -> std::string {
        // Helper to safely get value
        auto safe_get = [&](const std::string& section) -> std::string {
            if (ini.count(section) && ini[section].count(key)) {
                return ini[section][key].as<std::string>();
            }
            return "";
        };

        std::string val;
        // Try global/default section (empty string key sometimes used for global, 
        // but inifile-cpp might just use "" as a key if parsed that way? 
        // Typically global keys are in root map for some parsers, but here it's map<string, section>. 
        // Let's assume global keys might be under an empty section name if the parser supports it.
        val = safe_get("");
        if (!val.empty()) return val;

        val = safe_get("General");
        if (!val.empty()) return val;
        
        val = safe_get("config");
        if (!val.empty()) return val;
        
        return "";
    };

    std::string tmpl = get_val("template");
    if (!tmpl.empty()) site_ctx.config["template"] = tmpl;
    
    std::string out = get_val("output");
    if (!out.empty()) site_ctx.config["output"] = out;

    std::string assets = get_val("assets");
    if (!assets.empty()) site_ctx.config["assets"] = assets;
    
    std::string base = get_val("base_path");
    if (!base.empty()) site_ctx.config["base_path"] = base;

    // Set defaults/paths
    if (site_ctx.config.contains("template")) {
        site_ctx.template_path = site_ctx.config["template"].get<std::string>();
    } else {
        throw std::runtime_error("Config missing 'template'");
    }
    
    site_ctx.output_root = site_ctx.config.contains("output") ? fs::path(site_ctx.config["output"].get<std::string>()) : "output_site";
    site_ctx.assets_path = site_ctx.config.contains("assets") ? fs::path(site_ctx.config["assets"].get<std::string>()) : "";

    // Plugins Section
    if (ini.count("plugins")) {
        if (ini["plugins"].count("path")) {
             site_ctx.config["plugins_path"] = ini["plugins"]["path"].as<std::string>();
        }
        if (ini["plugins"].count("enabled")) {
             site_ctx.config["plugins_enabled"] = ini["plugins"]["enabled"].as<std::string>();
        }
    }
}

void SiteBuilder::register_plugins() {
    std::string path_str = site_ctx.config.value("plugins_path", "plugins");
    std::string enabled_str = site_ctx.config.value("plugins_enabled", "");
    
    fs::path plugin_dir = path_str;
    if (!fs::exists(plugin_dir)) {
        // Try relative to executable or current path
        // For now, just warn
        std::println(stderr, "Warning: Plugin directory '{}' does not exist.", plugin_dir.string());
    }

    if (enabled_str.empty()) {
        std::println("No plugins enabled in config.");
        return;
    }

    std::stringstream ss(enabled_str);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // Trim
        auto start = item.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        auto end = item.find_last_not_of(" \t");
        item = item.substr(start, end - start + 1);
        
        if (item.empty()) continue;
        
        // Expecting "markdown" -> "plugin_markdown.so" (PREFIX was removed in CMake)
        fs::path p = plugin_dir / std::format("plugin_{}.so", item);
        plugin_manager.load_plugin_from_file(p);
    }
}

// --- Logic ---

bool SiteBuilder::is_supported(const fs::path& p) {
    std::string ext = p.extension().string();
    return plugin_manager.get_renderer(ext) != nullptr;
}

DirNode SiteBuilder::scan_directory(const fs::path& current_path) {
    DirNode node;
    node.dir_name = current_path.filename().string();
    node.relative_path = fs::relative(current_path, site_ctx.source_root);
    if (node.relative_path == ".") node.relative_path = "";

    for (const auto& entry : fs::directory_iterator(current_path)) {
        std::string name = entry.path().filename().string();
        if (name.starts_with(".")) continue;

        if (entry.is_directory()) {
            if (name == "assets") continue;
            node.subdirs.push_back(scan_directory(entry.path()));
        } else if (entry.is_regular_file()) {
            if (is_supported(entry.path())) {
                node.files.push_back(entry.path().filename());
            }
        }
    }
    // Sort for consistent navigation
    std::ranges::sort(node.subdirs, {}, &DirNode::dir_name);
    std::ranges::sort(node.files);
    return node;
}

std::string SiteBuilder::generate_nav_html(const DirNode& node, const std::string& url_prefix, const fs::path& active_file, bool is_root) {
    std::stringstream html;
    html << "<ul class=\"nav-list\">\n";

    if (!is_root) {
        for (const auto& file : node.files) {
            std::string name_no_ext = file.stem().string();
            std::ranges::replace(name_no_ext, '_', ' ');

            // Target HTML filename
            fs::path target_file = file;
            target_file.replace_extension(".html");
            
            fs::path full_link_path = node.relative_path / target_file;
            std::string href = url_prefix + full_link_path.generic_string();
            
            std::string class_attr = (full_link_path == active_file) ? " class=\"active\"" : "";
            html << std::format("  <li><a href=\"{}\"{}>{}</a></li>\n", href, class_attr, name_no_ext);
        }
    }

    for (const auto& sub : node.subdirs) {
        html << std::format("  <li><strong>{}</strong>\n", sub.dir_name);
        html << generate_nav_html(sub, url_prefix, active_file, false);
        html << "  </li>\n";
    }
    html << "</ul>\n";
    return html.str();
}

void SiteBuilder::build() {
    if (site_ctx.source_root.empty()) {
         throw std::runtime_error("Source root not set");
    }

    std::println("Initializing...");
    plugin_manager.dispatch([this](auto& p){ p.on_init(site_ctx); });

    if (fs::exists(site_ctx.output_root)) fs::remove_all(site_ctx.output_root);
    fs::create_directories(site_ctx.output_root);

    if (!site_ctx.assets_path.empty() && fs::exists(site_ctx.assets_path)) {
        fs::create_directories(site_ctx.output_root / "assets");
        fs::copy(site_ctx.assets_path, site_ctx.output_root / "assets", fs::copy_options::recursive);
    }

    std::println("Scanning structure...");
    root_node = scan_directory(site_ctx.source_root);

    std::println("Generating pages...");
    process_node(root_node);
    
    plugin_manager.dispatch([this](auto& p){ p.on_post_build(site_ctx); });
    std::println("Done! Output in: {}", site_ctx.output_root.string());
}

void SiteBuilder::process_node(const DirNode& node) {
    for (const auto& file : node.files) {
        process_file(site_ctx.source_root / node.relative_path / file, node);
    }
    for (const auto& sub : node.subdirs) {
        process_node(sub);
    }
}

void SiteBuilder::process_file(const fs::path& file_path, const DirNode& current_node) {
    std::string ext = file_path.extension().string();
    auto renderer = plugin_manager.get_renderer(ext);
    
    if (!renderer) return;

    // Setup Context
    model::PageContext page_ctx;
    page_ctx.source_path = file_path;
    page_ctx.relative_path = fs::relative(file_path, site_ctx.source_root);
    page_ctx.output_path = site_ctx.output_root / page_ctx.relative_path;
    page_ctx.output_path.replace_extension(".html");

    fs::create_directories(page_ctx.output_path.parent_path());

    // Read Content
    page_ctx.raw_content = read_file(file_path);
    page_ctx.meta_data["title"] = file_path.stem().string();

    // Base Path & Navigation
    std::string back_prefix = "";
    for (const auto& _ : page_ctx.relative_path.parent_path()) back_prefix += "../";
    
    std::string nav_base = back_prefix; // Navigation links are relative
    std::string asset_base = back_prefix;

    if (site_ctx.config.contains("base_path")) {
        std::string bp = site_ctx.config["base_path"].get<std::string>();
        if (!bp.empty()) {
            if (!bp.ends_with('/')) bp += '/';
             asset_base = bp;
        }
    }
    
    page_ctx.meta_data["base_path"] = asset_base;

    // Generate Navigation
    fs::path current_active_file = page_ctx.relative_path;
    current_active_file.replace_extension(".html");
    page_ctx.meta_data["navigation"] = generate_nav_html(root_node, nav_base, current_active_file, true);

    
    // Plugin Hook: Pre-Render
    plugin_manager.dispatch([&](auto& p){ p.on_before_render(page_ctx); });

    // Render
    auto result = renderer->render(page_ctx);
    if (!result) {
        std::println(stderr, "Failed to render {}: {}", file_path.string(), result.error());
        return;
    }
    page_ctx.html_content = result.value();

    // Plugin Hook: Post-Render
    plugin_manager.dispatch([&](auto& p){ p.on_after_render(page_ctx); });

    // Apply Template
    try {
        auto tmpl = inja_env.parse_template(site_ctx.template_path.string());
        
        // Final render of content to resolve variables inside the page body
        std::string rendered_content = inja_env.render(page_ctx.html_content, page_ctx.meta_data);
        page_ctx.meta_data["content"] = rendered_content;
        
        std::string final_html = inja_env.render(tmpl, page_ctx.meta_data);
        write_file(page_ctx.output_path, final_html);
        std::println("Generated: {}", page_ctx.output_path.string());
        
    } catch (const std::exception& e) {
        std::println(stderr, "Template error in {}: {}", file_path.string(), e.what());
    }
}

} // namespace ssg::core
