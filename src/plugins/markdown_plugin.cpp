/**
 * SPDX-FileComment: Markdown Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file markdown_plugin.cpp
 * @brief Implementation of the Markdown Plugin using md4c.
 * @version 0.1.2
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "plugins/markdown_plugin.hpp"
#include <format>
#include <md4c.h>
#include <sstream>

namespace ssg::plugins {

class MarkdownRenderer : public core::IRenderer {
  core::IPluginHost &host;

  struct RenderState {
    core::IPluginHost &host;
    model::PageContext &ctx;
    std::ostream &out;
  };

  // --- Blocks ---

  static int enter_block(MD_BLOCKTYPE type, void *detail, void *userdata) {
    auto *state = static_cast<RenderState *>(userdata);
    if (state->host.dispatch_md_enter(type, detail, state->ctx, state->out))
      return 0;

    switch (type) {
    case MD_BLOCK_DOC:
      break; // Root
    case MD_BLOCK_QUOTE:
      state->out << "<blockquote>\n";
      break;
    case MD_BLOCK_UL:
      state->out << "<ul>\n";
      break;
    case MD_BLOCK_OL:
      state->out << "<ol>\n";
      break;
    case MD_BLOCK_LI:
      state->out << "<li>";
      break;
    case MD_BLOCK_HR:
      state->out << "<hr>\n";
      break;
    case MD_BLOCK_H: {
      auto *h_detail = static_cast<MD_BLOCK_H_DETAIL *>(detail);
      state->out << "<h" << h_detail->level << ">";
      break;
    }
    case MD_BLOCK_CODE:
      state->out << "<pre><code>";
      break;
    case MD_BLOCK_P:
      state->out << "<p>";
      break;

    // --- Table Support ---
    case MD_BLOCK_TABLE:
      state->out << "<table class=\"table\">\n";
      break;
    case MD_BLOCK_THEAD:
      state->out << "<thead>\n";
      break;
    case MD_BLOCK_TBODY:
      state->out << "<tbody>\n";
      break;
    case MD_BLOCK_TR:
      state->out << "<tr>\n";
      break;
    case MD_BLOCK_TH:
    case MD_BLOCK_TD: {
      auto *cell = static_cast<MD_BLOCK_TD_DETAIL *>(detail);
      state->out << (type == MD_BLOCK_TH ? "<th" : "<td");

      // Handle Alignment
      if (cell->align != MD_ALIGN_DEFAULT) {
        state->out << " style=\"text-align:";
        switch (cell->align) {
        case MD_ALIGN_LEFT:
          state->out << "left";
          break;
        case MD_ALIGN_CENTER:
          state->out << "center";
          break;
        case MD_ALIGN_RIGHT:
          state->out << "right";
          break;
        default:
          break;
        }
        state->out << "\"";
      }
      state->out << ">";
      break;
    }

    default:
      break;
    }
    return 0;
  }

  static int leave_block(MD_BLOCKTYPE type, void *detail, void *userdata) {
    auto *state = static_cast<RenderState *>(userdata);
    if (state->host.dispatch_md_leave(type, detail, state->ctx, state->out))
      return 0;

    switch (type) {
    case MD_BLOCK_DOC:
      break;
    case MD_BLOCK_QUOTE:
      state->out << "</blockquote>\n";
      break;
    case MD_BLOCK_UL:
      state->out << "</ul>\n";
      break;
    case MD_BLOCK_OL:
      state->out << "</ol>\n";
      break;
    case MD_BLOCK_LI:
      state->out << "</li>\n";
      break;
    case MD_BLOCK_H: {
      auto *h_detail = static_cast<MD_BLOCK_H_DETAIL *>(detail);
      state->out << "</h" << h_detail->level << ">\n";
      break;
    }
    case MD_BLOCK_CODE:
      state->out << "</code></pre>\n";
      break;
    case MD_BLOCK_P:
      state->out << "</p>\n";
      break;

    // --- Table Support ---
    case MD_BLOCK_TABLE:
      state->out << "</table>\n";
      break;
    case MD_BLOCK_THEAD:
      state->out << "</thead>\n";
      break;
    case MD_BLOCK_TBODY:
      state->out << "</tbody>\n";
      break;
    case MD_BLOCK_TR:
      state->out << "</tr>\n";
      break;
    case MD_BLOCK_TH:
      state->out << "</th>\n";
      break;
    case MD_BLOCK_TD:
      state->out << "</td>\n";
      break;

    default:
      break;
    }
    return 0;
  }

  // --- Spans (Links, Bold, Italic) ---

  static int enter_span(MD_SPANTYPE type, void *detail, void *userdata) {
    auto *state = static_cast<RenderState *>(userdata);

    switch (type) {
    case MD_SPAN_EM:
      state->out << "<em>";
      break;
    case MD_SPAN_STRONG:
      state->out << "<strong>";
      break;
    case MD_SPAN_A: {
      auto *a_detail = (MD_SPAN_A_DETAIL *)detail;
      std::string href(a_detail->href.text, a_detail->href.size);
      state->out << std::format("<a href=\"{}\">", href);
      break;
    }
    case MD_SPAN_CODE:
      state->out << "<code>";
      break;
    default:
      break;
    }
    return 0;
  }

  static int leave_span(MD_SPANTYPE type, void *detail, void *userdata) {
    auto *state = static_cast<RenderState *>(userdata);
    switch (type) {
    case MD_SPAN_EM:
      state->out << "</em>";
      break;
    case MD_SPAN_STRONG:
      state->out << "</strong>";
      break;
    case MD_SPAN_A:
      state->out << "</a>";
      break;
    case MD_SPAN_CODE:
      state->out << "</code>";
      break;
    default:
      break;
    }
    return 0;
  }

  // --- Text ---

  static int text(MD_TEXTTYPE type, const char *text, MD_SIZE size,
                  void *userdata) {
    auto *state = static_cast<RenderState *>(userdata);
    if (state->host.dispatch_md_text(type, text, size, state->ctx, state->out))
      return 0;

    if (type == MD_TEXT_HTML || type == MD_TEXT_ENTITY) {
      // Raw HTML or Entity -> Do not escape
      state->out.write(text, size);
    } else {
      // Basic HTML escaping
      for (MD_SIZE i = 0; i < size; ++i) {
        switch (text[i]) {
        case '<':
          state->out << "&lt;";
          break;
        case '>':
          state->out << "&gt;";
          break;
        case '&':
          state->out << "&amp;";
          break;
        case '"':
          state->out << "&quot;";
          break;
        default:
          state->out << text[i];
          break;
        }
      }
    }
    return 0;
  }

public:
  MarkdownRenderer(core::IPluginHost &h) : host(h) {}

  std::expected<std::string, std::string>
  render(model::PageContext &ctx) override {
    std::stringstream out;
    RenderState state{host, ctx, out};

    MD_PARSER parser = {0,          MD_DIALECT_GITHUB, enter_block, leave_block,
                        enter_span, leave_span,        text,        nullptr};

    int ret = md_parse(ctx.raw_content.c_str(), ctx.raw_content.size(), &parser,
                       &state);

    if (ret != 0)
      return std::unexpected("Markdown parsing failed");
    return out.str();
  }
};

std::unique_ptr<core::IRenderer>
MarkdownPlugin::create_renderer(const std::string &extension) {
  if (extension == ".md" || extension == ".markdown") {
    return std::make_unique<MarkdownRenderer>(host);
  }
  return nullptr;
}

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::MarkdownPlugin(*host);
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }