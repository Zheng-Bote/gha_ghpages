/**
 * SPDX-FileComment: Minify Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file minify_plugin.cpp
 * @brief Minifies HTML, CSS, and JS files in the output directory.
 * @version 0.1.1
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
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace ssg::plugins {

class MinifyPlugin : public core::IPlugin {
public:
  std::string name() const override { return "Minifier"; }
  std::string version() const override { return "0.1.1"; }
  std::string description() const override {
    return "Minifies HTML and CSS output";
  }

  void on_post_build(model::SiteContext &ctx) override {
    std::cout << "Minifying assets in " << ctx.output_root << "..."
              << std::endl;

    size_t count = 0;
    for (auto &p :
         std::filesystem::recursive_directory_iterator(ctx.output_root)) {
      if (p.is_regular_file()) {
        std::string ext = p.path().extension().string();
        if (ext == ".html" || ext == ".htm") {
          minify_file(p.path(), &MinifyPlugin::minify_html);
          count++;
        } else if (ext == ".css") {
          minify_file(p.path(), &MinifyPlugin::minify_css);
          count++;
        }
        // JS minification with Regex is too risky/complex for a simple plugin.
        // We skip JS to avoid breaking functionality.
      }
    }
    std::cout << "Minified " << count << " files." << std::endl;
  }

private:
  void minify_file(const std::filesystem::path &path,
                   std::string (MinifyPlugin::*minifier)(const std::string &)) {
    std::ifstream in(path, std::ios::binary);
    if (!in)
      return;
    std::string content((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    in.close();

    std::string minified = (this->*minifier)(content);

    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    out << minified;
  }

  std::string minify_css(const std::string &input) {
    std::string s = input;

    // 1. Remove comments /* ... */
    s = std::regex_replace(s, std::regex(R"(/\*[\s\S]*?\*/)"), "");

    // 2. Normalize whitespace (newlines/tabs to space)
    std::replace(s.begin(), s.end(), '\n', ' ');
    std::replace(s.begin(), s.end(), '\r', ' ');
    std::replace(s.begin(), s.end(), '\t', ' ');

    // 3. Collapse multiple spaces
    s = std::regex_replace(s, std::regex(R"(\s+)"), " ");

    // 4. Remove space around delimiters { } : ; ,
    s = std::regex_replace(s, std::regex(R"(\s*([{}:;,])\s*)"), "$1");

    return s;
  }

  std::string minify_html(const std::string &input) {
    std::string s = input;

    // 1. Remove comments <!-- ... -->
    // Note: This removes all comments. If IE conditionals are needed, this
    // regex needs adjustment.
    s = std::regex_replace(s, std::regex(R"(<!--[\s\S]*?-->)"), "");

    // 2. Collapse whitespace between tags: >  < becomes ><
    // This is generally safe for block elements, but can affect inline-block
    // layout. However, for a standard doc site, it usually works fine and saves
    // space.
    s = std::regex_replace(s, std::regex(R"(>\s+<)"), "><");

    return s;
  }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::MinifyPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }
