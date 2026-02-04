/**
 * SPDX-FileComment: Breadcrumb Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file breadcrumb_plugin.cpp
 * @brief Generates breadcrumb navigation based on directory structure.
 * @version 0.1.0
 * @date 2026-02-04
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cctype>

namespace ssg::plugins {

class BreadcrumbPlugin : public core::IPlugin {
    
    std::string capitalize(std::string text) {
        if (!text.empty()) {
            text[0] = std::toupper(text[0]);
        }
        return text;
    }

public:
    std::string name() const override { return "Breadcrumb Navigation"; }
    std::string version() const override { return "1.0.0"; }
    std::string description() const override { return "Generates breadcrumb HTML"; }

    void on_before_render(model::PageContext& ctx) override {
        // Path relative to source root, e.g., "docs/architecture/overview.md"
        std::filesystem::path p = ctx.relative_path;
        
        // Count total components
        auto dist = std::distance(p.begin(), p.end());
        int depth = (dist > 0) ? (dist - 1) : 0;

        std::stringstream html;
        html << "<div class=\"breadcrumb mb-3\">\n";

        // 1. Home Link
        std::string home_link = "";
        for (int i = 0; i < depth; ++i) home_link += "../";
        home_link += "index.html";

        html << "  <a href=\"" << home_link << "\">Home</a>\n";

        // 2. Intermediate Folders
        int current_depth = depth;
        for (auto it = p.begin(); it != p.end(); ++it) {
            // Check if this is the last element (filename)
            auto next_it = it;
            ++next_it;
            if (next_it == p.end()) break; 

            std::string folder_name = capitalize(it->string());
            current_depth--; // Go one level down
            
            // Link to '#' because not every folder has an index.html
            std::string link = "#";

            html << "  <span class=\"breadcrumb-separator\">/</span>\n";
            html << "  <a href=\"" << link << "\">" << folder_name << "</a>\n";
        }

        // 3. Current Page (Active)
        std::string title = ctx.source_path.stem().string();
        if (ctx.meta_data.contains("title")) {
            title = ctx.meta_data["title"].get<std::string>();
        }
        
        html << "  <span class=\"breadcrumb-separator\">/</span>\n";
        html << "  <span>" << title << "</span>\n";

        html << "</div>\n";

        ctx.meta_data["breadcrumbs"] = html.str();
    }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::BreadcrumbPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}
