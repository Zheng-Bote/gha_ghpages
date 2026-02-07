/**
 * SPDX-FileComment: Sitemap Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file sitemap_plugin.cpp
 * @brief Generates sitemap.xml and a rendered HTML sitemap.
 * @version 0.1.0
 * @date 2026-02-04
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <nlohmann/json.hpp>
#include <inja.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <format>
#include <algorithm>

namespace ssg::plugins {

using json = nlohmann::json;

struct SitemapEntry {
    std::string title;
    std::string url;      // Relative to output root (e.g., "docs/arch.html")
};

class SitemapPlugin : public core::IPlugin {
    std::vector<SitemapEntry> entries;
    std::string cached_nav; // Store one instance of navigation to reuse (ignoring relative link issues for now or fix later)

public:
    std::string name() const override { return "Sitemap Generator"; }
    std::string version() const override { return "1.0.0"; }
    std::string description() const override { return "Generates sitemap.xml and Sitemap/index.html"; }

    void on_init(model::SiteContext& ctx) override {
        entries.clear();
        cached_nav.clear();
    }

    void on_after_render(model::PageContext& ctx) override {
        SitemapEntry entry;
        
        // 1. Title
        if (ctx.meta_data.contains("title")) {
            entry.title = ctx.meta_data["title"].get<std::string>();
        } else {
            entry.title = ctx.source_path.stem().string();
        }
        
        // 2. Relative URL
        std::filesystem::path p = ctx.relative_path;
        p.replace_extension(".html");
        entry.url = p.string();
        #ifdef _WIN32
        std::replace(entry.url.begin(), entry.url.end(), '\\', '/');
        #endif

        entries.push_back(entry);
    }

    void on_post_build(model::SiteContext& ctx) override {
        // --- 1. XML Sitemap ---
        std::string base_url = "http://example.com/";
        if (ctx.config.contains("base_path")) {
             std::string bp = ctx.config["base_path"].get<std::string>();
             if (!bp.empty()) {
                 base_url = bp;
                 if (!base_url.ends_with('/')) base_url += '/';
             }
        }

        std::stringstream xml;
        xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        xml << "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";
        
        for (const auto& e : entries) {
            xml << "  <url>\n";
            xml << "    <loc>" << base_url << e.url << "</loc>\n";
            xml << "  </url>\n";
        }
        xml << "</urlset>";

        std::filesystem::path xml_path = ctx.output_root / "sitemap.xml";
        std::ofstream xml_out(xml_path);
        if (xml_out) {
            xml_out << xml.str();
            std::cout << "Generated sitemap.xml" << std::endl;
        }

        // --- 2. HTML Sitemap ---
        
        // Sort entries by title for better readability in HTML
        std::vector<SitemapEntry> sorted_entries = entries;
        std::sort(sorted_entries.begin(), sorted_entries.end(), [](const auto& a, const auto& b){
            return a.title < b.title;
        });

        std::stringstream html;
        html << "<div class=\"sitemap-container\">\n";
        html << "<h1>Sitemap</h1>\n";
        html << "<ul>\n";
        
        for (const auto& e : sorted_entries) {
             // Link from "Sitemap/index.html" to "docs/..." needs "../"
             html << std::format("<li><a href=\"../{}\">{}</a></li>\n", e.url, e.title);
        }
        html << "</ul>\n";
        html << "</div>\n";

        std::filesystem::path sitemap_dir = ctx.output_root / "Sitemap";
        std::filesystem::create_directories(sitemap_dir);
        
        if (std::filesystem::exists(ctx.template_path)) {
            try {
                inja::Environment env;
                auto tmpl = env.parse_template(ctx.template_path.string());
                
                json data;
                data["title"] = "Sitemap";
                data["content"] = html.str();
                data["base_path"] = "../"; // We are in Sitemap/ folder, so one level up
                
                // Navigation
                data["navigation"] = "<ul class=\"nav-list\"><li><a href=\"../index.html\">Back to Home</a></li></ul>"; 

                // Breadcrumbs (Required by template)
                data["breadcrumbs"] = R"(<div class="breadcrumb mb-3"><a href="../index.html">Home</a> <span class="breadcrumb-separator">/</span> <span>Sitemap</span></div>)";

                // Reading Time (Required by template)
                data["show_reading_time"] = false;
                data["reading_time"] = "";

                std::string res = env.render(tmpl, data);
                
                std::ofstream html_out(sitemap_dir / "index.html");
                if (html_out) {
                    html_out << res;
                    std::cout << "Generated Sitemap/index.html" << std::endl;
                }
                
            } catch (const std::exception& e) {
                std::cerr << "Sitemap template error: " << e.what() << std::endl;
            }
        }
    }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::SitemapPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}
