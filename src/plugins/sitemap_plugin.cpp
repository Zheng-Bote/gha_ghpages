/**
 * SPDX-FileComment: Sitemap Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file sitemap_plugin.cpp
 * @brief Generates sitemap.xml and folder-based index.html files.
 * @version 0.2.1
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <inja.hpp>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <string>
#include <vector>

namespace ssg::plugins {

using json = nlohmann::json;
namespace fs = std::filesystem;

struct SitemapEntry {
  std::string title;
  std::string url;      // Relative to output root (e.g., "docs/arch.html")
  std::string filename; // Just the filename (e.g., "arch.html")
};

// Helper struct to organize content per directory
struct DirContent {
  std::vector<SitemapEntry> files;
  std::set<std::string> subdirs;
};

class SitemapPlugin : public core::IPlugin {
  std::vector<SitemapEntry> entries;

public:
  std::string name() const override { return "Sitemap Generator"; }
  std::string version() const override { return "0.2.1"; }
  std::string description() const override {
    return "Generates sitemap.xml and folder-based index.html files";
  }

  void on_init(model::SiteContext &ctx) override { entries.clear(); }

  void on_after_render(model::PageContext &ctx) override {
    SitemapEntry entry;

    // 1. Title
    if (ctx.meta_data.contains("title")) {
      entry.title = ctx.meta_data["title"].get<std::string>();
    } else {
      entry.title = ctx.source_path.stem().string();
    }

    // 2. Relative URL
    fs::path p = ctx.relative_path;
    p.replace_extension(".html");

    entry.url = p.string();
    entry.filename = p.filename().string();

#ifdef _WIN32
    std::replace(entry.url.begin(), entry.url.end(), '\\', '/');
#endif

    entries.push_back(entry);
  }

  void on_post_build(model::SiteContext &ctx) override {
    // --- 1. XML Sitemap (Global) ---
    generate_xml_sitemap(ctx);

    // --- 2. HTML Folder Indexes ---
    generate_folder_indexes(ctx);
  }

private:
  void generate_xml_sitemap(model::SiteContext &ctx) {
    std::string base_url = "http://example.com/";
    if (ctx.config.contains("base_path")) {
      std::string bp = ctx.config["base_path"].get<std::string>();
      if (!bp.empty()) {
        base_url = bp;
        if (!base_url.ends_with('/'))
          base_url += '/';
      }
    }

    std::stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";

    for (const auto &e : entries) {
      xml << "  <url>\n";
      xml << "    <loc>" << base_url << e.url << "</loc>\n";
      xml << "  </url>\n";
    }
    xml << "</urlset>";

    fs::path xml_path = ctx.output_root / "sitemap.xml";
    std::ofstream xml_out(xml_path);
    if (xml_out) {
      xml_out << xml.str();
      std::cout << "Generated sitemap.xml" << std::endl;
    }
  }

  void generate_folder_indexes(model::SiteContext &ctx) {
    // Map: Directory Path (relative string) -> Content
    std::map<std::string, DirContent> dir_map;

    // Populate map from entries
    for (const auto &e : entries) {
      fs::path full_rel_path(e.url);
      fs::path parent = full_rel_path.parent_path();

      // Normalize path string
      std::string parent_str = parent.string();
#ifdef _WIN32
      std::replace(parent_str.begin(), parent_str.end(), '\\', '/');
#endif
      if (parent_str == ".")
        parent_str = ""; // Root handling

      // Add file to its directory
      dir_map[parent_str].files.push_back(e);

      // Register directories recursively up to root
      fs::path current = parent;
      while (current.has_parent_path() &&
             current != current.parent_path()) { // Check paths like a/b/c
        fs::path upper = current.parent_path();
        std::string upper_str = upper.string();
        if (upper_str == ".")
          upper_str = "";

#ifdef _WIN32
        std::replace(upper_str.begin(), upper_str.end(), '\\', '/');
#endif

        // Add subdir name to parent
        dir_map[upper_str].subdirs.insert(current.filename().string());

        current = upper;
        if (current.string().empty() || current.string() == ".")
          break;
      }
      // Ensure root knows about top-level dirs
      if (!parent_str.empty()) {
        // FIX: removed ->path(), used dereference *
        fs::path top = *fs::path(parent_str).begin();

        if (top.string() != parent_str) {
          // Already handled by loop usually
        } else {
          dir_map[""].subdirs.insert(top.string());
        }
      }
    }

    // Render index.html for each directory
    for (auto &[dir_path, content] : dir_map) {

      // SKIP: Assets folder
      if (dir_path.rfind("assets", 0) == 0)
        continue;

      fs::path out_dir = ctx.output_root / dir_path;
      fs::path index_file = out_dir / "index.html";

      // SKIP: If index.html already exists (e.g. from markdown source)
      if (fs::exists(index_file))
        continue;

      // Sort for UI
      std::sort(content.files.begin(), content.files.end(),
                [](const auto &a, const auto &b) { return a.title < b.title; });

      // Calculate base_path (../../) for CSS/JS
      fs::path p_depth(dir_path);
      std::string relative_base = "";
      int depth = 0;
      if (!dir_path.empty()) {
        for (const auto &part : p_depth)
          depth++;
      }
      for (int i = 0; i < depth; ++i)
        relative_base += "../";
      if (relative_base.empty())
        relative_base = "./";

      // Generate HTML Content
      std::stringstream html;
      html << "<div class=\"sitemap-container\">\n";
      html << "<h1>Index of /" << dir_path << "</h1>\n";
      html << "<ul class=\"directory-list\">\n";

      // 1. Link Up (if not root)
      if (!dir_path.empty()) {
        html << "<li><strong><a href=\"../index.html\">📂 .. (Parent "
                "Directory)</a></strong></li>\n";
      }

      // 2. Subdirectories
      for (const auto &subdir : content.subdirs) {
        // Ignore assets folder in lists as well
        if (subdir == "assets")
          continue;
        html << "<li><strong><a href=\"" << subdir << "/index.html\">📂 "
             << subdir << "/</a></strong></li>\n";
      }

      // 3. Files
      for (const auto &f : content.files) {
        if (f.filename == "index.html")
          continue; // Don't link to self
        html << "<li><a href=\"" << f.filename << "\">📄 " << f.title
             << "</a></li>\n";
      }

      html << "</ul>\n</div>\n";

      // Render via Inja
      if (fs::exists(ctx.template_path)) {
        try {
          inja::Environment env;
          auto tmpl = env.parse_template(ctx.template_path.string());

          json data;
          data["title"] =
              dir_path.empty() ? "Home Index" : "Index of " + dir_path;
          data["content"] = html.str();
          data["base_path"] = relative_base;

          // Simple Navigation
          data["navigation"] = "<ul class=\"nav-list\"><li><a href=\"" +
                               relative_base +
                               "index.html\">Home</a></li></ul>";

          // Breadcrumbs
          data["breadcrumbs"] = std::format(
              R"(<div class="breadcrumb mb-3"><a href="{}index.html">Home</a> <span class="breadcrumb-separator">/</span> <span>{}</span></div>)",
              relative_base, dir_path.empty() ? "Root" : dir_path);

          data["show_reading_time"] = false;
          data["reading_time"] = "";

          // Ensure directory exists
          fs::create_directories(out_dir);

          std::string res = env.render(tmpl, data);
          std::ofstream html_out(index_file);
          if (html_out) {
            html_out << res;
            std::cout << "Generated auto-index for: "
                      << (dir_path.empty() ? "ROOT" : dir_path) << std::endl;
          }

        } catch (const std::exception &e) {
          std::cerr << "Template error in sitemap plugin: " << e.what()
                    << std::endl;
        }
      }
    }
  }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::SitemapPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }