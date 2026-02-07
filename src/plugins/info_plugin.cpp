/**
 * SPDX-FileComment: Info Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file info_plugin.cpp
 * @brief Processes GFM Alerts and Emojis with robust tag matching.
 * @version 0.1.3
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace ssg::plugins {

class InfoPlugin : public core::IPlugin {
public:
  std::string name() const override { return "Info & Alerts"; }
  std::string version() const override { return "0.1.3"; }
  std::string description() const override {
    return "Processes GFM-style alerts and specific emojis.";
  }

  void on_after_render(model::PageContext &ctx) override {
    process_alerts(ctx.html_content);
    process_emojis(ctx.html_content);
  }

private:
  void process_alerts(std::string &html) {
    // Order matters for overlapping types if any, but these are unique.
    static const std::vector<std::pair<std::string, std::string>> alerts = {
        {"NOTE", "info-msg"},
        {"TIP", "success-msg"},
        {"IMPORTANT", "info-msg"},
        {"WARNING", "warning-msg"},
        {"CAUTION", "error-msg"}};

    for (const auto &[type, css_class] : alerts) {
      std::string marker = "[!" + type + "]";
      size_t search_pos = 0;

      while ((search_pos = html.find(marker, search_pos)) !=
             std::string::npos) {
        // Find the opening blockquote that contains this marker
        size_t bq_start = html.rfind("<blockquote>", search_pos);
        if (bq_start == std::string::npos) {
          search_pos += marker.length();
          continue;
        }

        // Ensure the blockquote hasn't already been closed before the marker
        size_t bq_end_check = html.find("</blockquote>", bq_start);
        if (bq_end_check != std::string::npos && bq_end_check < search_pos) {
          search_pos += marker.length();
          continue;
        }

        // Find the matching end tag for this specific blockquote
        size_t bq_end = find_matching_close_tag(html, bq_start);
        if (bq_end == std::string::npos) {
          search_pos += marker.length();
          continue;
        }

        // Transform back-to-front to keep indices valid during the process
        // 1. Replace closing tag (13 chars -> 6 chars)
        html.replace(bq_end, 13, "</div>");

        // 2. Remove marker and a potential following space
        size_t erase_len = marker.length();
        if (search_pos + erase_len < html.length() &&
            html[search_pos + erase_len] == ' ') {
          erase_len++;
        }
        html.erase(search_pos, erase_len);

        // 3. Replace opening tag (12 chars -> varies)
        std::string open_tag = "<div class=\"" + css_class + "\">";
        html.replace(bq_start, 12, open_tag);

        // Move search position past the transformed block
        search_pos = bq_start + open_tag.length();
      }
    }
  }

  /**
   * @brief Finds the matching </blockquote> for an opening tag, handling
   * nesting.
   */
  size_t find_matching_close_tag(const std::string &html, size_t start_pos) {
    int depth = 0;
    size_t pos = start_pos;
    while (pos < html.length()) {
      size_t next_open = html.find("<blockquote", pos);
      size_t next_close = html.find("</blockquote>", pos);

      if (next_close == std::string::npos)
        return std::string::npos;

      if (next_open != std::string::npos && next_open < next_close) {
        depth++;
        pos = next_open + 11; // Move past "<blockquote"
      } else {
        depth--;
        if (depth == 0)
          return next_close;
        pos = next_close + 13; // Move past "</blockquote>"
      }
    }
    return std::string::npos;
  }

  void process_emojis(std::string &html) {
    static const std::map<std::string, std::string> emoji_map = {
        {":arrow_right:", "&#10145;"},
        {":arrow_left:", "&#11013;"},
        {":warning:", "&#9888;"}};

    for (const auto &[code, replacement] : emoji_map) {
      size_t pos = 0;
      while ((pos = html.find(code, pos)) != std::string::npos) {
        html.replace(pos, code.length(), replacement);
        pos += replacement.length();
      }
    }
  }
};

} // namespace ssg::plugins

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::InfoPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }