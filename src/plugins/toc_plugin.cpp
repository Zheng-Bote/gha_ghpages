/**
 * SPDX-FileComment: TOC Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file toc_plugin.cpp
 * @brief Implementation of the TOC Plugin.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "plugins/toc_plugin.hpp"
#include <cctype>
#include <format>
#include <iostream>
#include <md4c.h>

namespace ssg::plugins {

std::string TocPlugin::slugify(const std::string &text) {
  std::string slug;
  for (char c : text) {
    if (std::isalnum(c)) {
      slug += std::tolower(c);
    } else if (c == ' ' || c == '-') {
      if (!slug.empty() && slug.back() != '-') {
        slug += '-';
      }
    }
  }
  // Trim trailing dashes
  while (!slug.empty() && slug.back() == '-') {
    slug.pop_back();
  }
  if (slug.empty())
    slug = "section";

  // Deduplication
  std::string final_slug = slug;
  int counter = 1;
  while (used_ids.contains(final_slug)) {
    final_slug = std::format("{}-", slug, counter++);
  }
  used_ids.insert(final_slug);
  return final_slug;
}

void TocPlugin::on_init(model::SiteContext &ctx) {
  // Check config if needed
}

void TocPlugin::on_before_render(model::PageContext &ctx) {
  state = State{};
  used_ids.clear();
  ctx.headers.clear();
}

bool TocPlugin::on_md_block_enter(MD_BLOCKTYPE type, void *detail,
                                  model::PageContext &ctx, std::ostream &out) {
  if (type == MD_BLOCK_H) {
    auto *h_detail = static_cast<MD_BLOCK_H_DETAIL *>(detail);
    if (h_detail->level >= 2 && h_detail->level <= 4) {
      state.in_header = true;
      state.current_level = h_detail->level;
      state.buffer.str("");
      state.buffer.clear();
      return true; // Suppress default rendering
    }
  }
  return false;
}

bool TocPlugin::on_md_text(MD_TEXTTYPE type, const char *text, MD_SIZE size,
                           model::PageContext &ctx, std::ostream &out) {
  if (state.in_header) {
    state.buffer.write(text, size);
    return true; // Suppress default rendering, we buffer it
  }
  return false;
}

bool TocPlugin::on_md_block_leave(MD_BLOCKTYPE type, void *detail,
                                  model::PageContext &ctx, std::ostream &out) {
  if (type == MD_BLOCK_H) {
    auto *h_detail = static_cast<MD_BLOCK_H_DETAIL *>(detail);
    if (h_detail->level >= 2 && h_detail->level <= 4) {
      std::string text = state.buffer.str();
      std::string id = slugify(text);

      // 1. Store for TOC generation
      ctx.headers.push_back({state.current_level, text, id});

      // 2. Render HTML with ID
      out << std::format("<h{} id=\"{}\">{}</h{}>\n", state.current_level, id,
                         text, state.current_level);

      state.in_header = false;
      return true;
    }
  }
  return false;
}

void TocPlugin::on_after_render(model::PageContext &ctx) {
  if (ctx.headers.empty())
    return;

  std::stringstream html;
  html << "<div class=\"toc\">\n";
  html << "<h3>Table of Contents</h3>\n";

  // Initialize with the level of the first header to avoid empty opening lists
  int current_level = ctx.headers[0].level;
  html << "<ul class=\"toc-level-" << current_level << "\">\n";

  for (const auto &h : ctx.headers) {
    if (h.level > current_level) {
      while (h.level > current_level) {
        current_level++;
        html << "<ul class=\"toc-level-" << current_level << "\">\n";
      }
    } else if (h.level < current_level) {
      while (h.level < current_level) {
        html << "</ul>\n";
        current_level--;
      }
    }
    // Now we are at the correct level, write the item
    html << std::format("<li><a href=\"#{}\">{}</a></li>\n", h.id, h.text);
  }

  // Close remaining lists back to the starting level
  // Note: We don't know the absolute start level anymore easily unless we
  // stored it, but we can just close until we match the stack depth? Actually,
  // we just need to close everything we opened. But wait, the first <ul> was
  // opened manually. If we end at level 3, and started at level 1. We need to
  // close 3 and 2. The 1 is closed at the very end.

  while (current_level > ctx.headers[0].level) {
    html << "</ul>\n";
    current_level--;
  }
  html << "</ul>\n"; // Close the root list
  html << "</div>\n";

  // Inject into metadata
  ctx.meta_data["toc"] = html.str();

  // Also support placeholder replacement for legacy compat
  std::string placeholder_start = "<!-- START doctoc generated TOC";
  std::string placeholder_end = "<!-- END doctoc generated TOC";

  size_t start = ctx.html_content.find(placeholder_start);
  size_t end = ctx.html_content.find(placeholder_end);

  // ... (previous content)

  if (start != std::string::npos && end != std::string::npos && end > start) {
    size_t content_start = ctx.html_content.find("-->", start) + 3;
    if (content_start < end) {
      ctx.html_content.replace(content_start, end - content_start,
                               "\n" + html.str() + "\n");
    }
  }
}

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::TocPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }