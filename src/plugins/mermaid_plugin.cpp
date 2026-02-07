/**
 * SPDX-FileComment: Mermaid Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file mermaid_plugin.cpp
 * @brief Renders Mermaid diagrams and injects necessary JS.
 * @version 0.1.0
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <iostream>
#include <string>
#include <string_view>

namespace ssg::plugins {

class MermaidPlugin : public core::IPlugin {
  bool in_mermaid_block = false;

public:
  std::string name() const override { return "Mermaid Diagram Support"; }
  std::string version() const override { return "0.1.0"; }
  std::string description() const override {
    return "Renders mermaid code blocks as diagrams";
  }

  void on_before_render(model::PageContext &ctx) override {
    in_mermaid_block = false;
    ctx.plugin_data["has_mermaid"] = false;
  }

  bool on_md_block_enter(MD_BLOCKTYPE type, void *detail,
                         model::PageContext &ctx, std::ostream &out) override {
    if (type == MD_BLOCK_CODE) {
      auto *code_detail = static_cast<MD_BLOCK_CODE_DETAIL *>(detail);
      if (code_detail->lang.text != nullptr) {
        std::string_view lang(code_detail->lang.text, code_detail->lang.size);
        if (lang == "mermaid") {
          in_mermaid_block = true;
          ctx.plugin_data["has_mermaid"] = true;
          out << "<div class=\"mermaid\">\n";
          return true; // Suppress default <pre><code>
        }
      }
    }
    return false;
  }

  bool on_md_text(MD_TEXTTYPE type, const char *text, MD_SIZE size,
                  model::PageContext &ctx, std::ostream &out) override {
    if (in_mermaid_block) {
      // Write escaped text so the browser doesn't consume < and > as tags
      for (MD_SIZE i = 0; i < size; ++i) {
        switch (text[i]) {
        case '<':
          out << "&lt;";
          break;
        case '>':
          out << "&gt;";
          break;
        case '&':
          out << "&amp;";
          break;
        default:
          out.put(text[i]);
          break;
        }
      }
      return true; // Suppress default processing
    }
    return false;
  }

  bool on_md_block_leave(MD_BLOCKTYPE type, void *detail,
                         model::PageContext &ctx, std::ostream &out) override {
    if (type == MD_BLOCK_CODE && in_mermaid_block) {
      out << "</div>\n";
      in_mermaid_block = false;
      return true; // Suppress default </code></pre>
    }
    return false;
  }

  void on_after_render(model::PageContext &ctx) override {
    // Inject script if mermaid was used
    if (ctx.plugin_data.contains("has_mermaid") &&
        std::any_cast<bool>(ctx.plugin_data["has_mermaid"])) {
      std::string script = R"(
<script type="module">
  import mermaid from 'https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.esm.min.mjs';
  mermaid.initialize({ startOnLoad: true });
</script>
)";
      // Append to content so it's included in {{ content }}
      ctx.html_content += script;
    }
  }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::MermaidPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }
