/**
 * SPDX-FileComment: Search Index Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file search_plugin.cpp
 * @brief Generates a JSON search index from processed pages.
 * @version 0.1.0
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <vector>

namespace ssg::plugins {

using json = nlohmann::json;

struct SearchEntry {
  std::string title;
  std::string url;
  std::string content; // Plain text snippet
};

class SearchIndexPlugin : public core::IPlugin {
  std::vector<SearchEntry> entries;

  // Helper to strip HTML tags
  std::string strip_html(const std::string &html) {
    // Regex is simple but sufficient for stripping tags for search index
    std::regex tag_regex("<[^>]*>");
    return std::regex_replace(html, tag_regex, " ");
  }

  // Normalize whitespace (remove newlines, multiple spaces)
  std::string normalize_space(std::string text) {
    std::regex space_regex("\\s+");
    return std::regex_replace(text, space_regex, " ");
  }

public:
  std::string name() const override { return "Search Index Generator"; }
  std::string version() const override { return "0.1.0"; }
  std::string description() const override {
    return "Generates search.json for client-side search";
  }

  void on_init(model::SiteContext &ctx) override { entries.clear(); }

  void on_after_render(model::PageContext &ctx) override {
    SearchEntry entry;

    // 1. Title
    if (ctx.meta_data.contains("title")) {
      entry.title = ctx.meta_data["title"].get<std::string>();
    } else {
      entry.title = ctx.source_path.stem().string();
    }

    // 2. URL
    // Convert relative path to web-friendly URL
    std::filesystem::path p = ctx.relative_path;
    p.replace_extension(".html");
    entry.url = p.string();

// Ensure forward slashes for web URLs
#ifdef _WIN32
    std::replace(entry.url.begin(), entry.url.end(), '\\', '/');
#endif

    // 3. Content
    // We use the HTML content because it has the final text (after
    // markdown/asciidoc processing)
    std::string plain = strip_html(ctx.html_content);
    entry.content = normalize_space(plain);

    entries.push_back(entry);
  }

  void on_post_build(model::SiteContext &ctx) override {
    json j_out = json::array();
    for (const auto &e : entries) {
      j_out.push_back(
          {{"title", e.title}, {"url", e.url}, {"content", e.content}});
    }

    std::filesystem::path out_file = ctx.output_root / "search.json";
    std::ofstream out(out_file);
    if (out) {
      out << j_out.dump(0); // Compact JSON
      std::cout << "Generated search.json with " << entries.size()
                << " entries." << std::endl;
    } else {
      std::cerr << "Failed to write search.json to " << out_file.string()
                << std::endl;
    }
  }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::SearchIndexPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }
