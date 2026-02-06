/**
 * SPDX-FileComment: TOC Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file toc_plugin.cpp
 * @brief Implementation of the TOC Plugin with support for HTML and Markdown.
 * @version 0.1.2
 * @date 2026-02-06
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
#include <regex>

namespace ssg::plugins {

std::string TocPlugin::slugify(const std::string &text) {
  std::string slug;
  // Remove HTML tags from text if any (e.g. from raw HTML headers)
  std::string clean_text = std::regex_replace(text, std::regex("<[^>]*>"), "");

  for (char c : clean_text) {
    if (std::isalnum(static_cast<unsigned char>(c))) {
      slug += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
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
    final_slug = std::format("{}-{}", slug, counter++);
  }
  used_ids.insert(final_slug);
  return final_slug;
}

void TocPlugin::on_init(model::SiteContext &ctx) {}

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
    return true; // Suppress default rendering
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

      ctx.headers.push_back({state.current_level, text, id});
      out << std::format("<h{} id=\"{}\">{}</h{}>\n", state.current_level, id,
                         text, state.current_level);

      state.in_header = false;
      return true;
    }
  }
  return false;
}

void TocPlugin::on_after_render(model::PageContext &ctx) {
  // 1. If headers list is empty, it's likely a non-Markdown file or raw HTML.
  //    Scan the generated HTML content for headers.
  if (ctx.headers.empty()) {
    std::regex header_regex(R"(<h([2-4])([^>]*)>(.*?)</h\1>)", std::regex_constants::icase);
    std::regex id_regex(R"(id=["']([^"']*)["'])", std::regex_constants::icase);
    
    std::string new_html;
    new_html.reserve(ctx.html_content.size());
    
    auto words_begin = std::sregex_iterator(ctx.html_content.begin(), ctx.html_content.end(), header_regex);
    auto words_end = std::sregex_iterator();
    
    size_t last_pos = 0;
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;
      new_html.append(ctx.html_content, last_pos, match.position() - last_pos);
      
      int level = std::stoi(match[1].str());
      std::string attrs = match[2].str();
      std::string content = match[3].str();
      
      std::string id;
      std::smatch id_match;
      if (std::regex_search(attrs, id_match, id_regex)) {
        id = id_match[1].str();
        // Register existing ID to avoid collisions
        used_ids.insert(id);
      } else {
        id = slugify(content);
        // Inject ID into the tag
        attrs += std::format(" id=\"{}\"", id);
      }
      
      ctx.headers.push_back({level, content, id});
      new_html += std::format("<h{}{}>{}</h{}>", level, attrs, content, level);
      
      last_pos = match.position() + match.length();
    }
    new_html.append(ctx.html_content, last_pos, std::string::npos);
    ctx.html_content = new_html;
  }

  if (ctx.headers.empty())
    return;

  // 2. Generate TOC HTML
  std::stringstream html;
  html << "<div class=\"toc\">\n";
  html << "<h3>Table of Contents</h3>\n";

  int start_level = ctx.headers[0].level;
  int current_level = start_level;
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
    html << std::format("<li><a href=\"#{}\">{}</a></li>\n", h.id, h.text);
  }

  while (current_level > start_level) {
    html << "</ul>\n";
    current_level--;
  }
  html << "</ul>\n</div>\n";

  ctx.meta_data["toc"] = html.str();

  // 3. Support legacy placeholder replacement
  std::string placeholder_start = "<!-- START doctoc generated TOC";
  std::string placeholder_end = "<!-- END doctoc generated TOC";

  size_t start = ctx.html_content.find(placeholder_start);
  size_t end = ctx.html_content.find(placeholder_end);

  if (start != std::string::npos && end != std::string::npos && end > start) {
    size_t content_start = ctx.html_content.find("-->", start) + 3;
    if (content_start < end) {
      ctx.html_content.replace(content_start, end - content_start,
                               "\n" + html.str() + "\n");
    }
  }
}

} // namespace ssg::plugins

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::TocPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }
