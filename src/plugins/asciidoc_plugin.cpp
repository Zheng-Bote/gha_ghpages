/**
 * SPDX-FileComment: AsciiDoc Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file asciidoc_plugin.cpp
 * @brief Renderer for AsciiDoc files using the 'asciidoctor' CLI.
 * needs apt-get install asciidoctor
 * @version 0.1.0
 * @date 2026-02-04
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <array>
#include <cstdio>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <string>

namespace ssg::plugins {

class AsciiDocRenderer : public core::IRenderer {
public:
  std::expected<std::string, std::string>
  render(model::PageContext &ctx) override {
    // We use the 'asciidoctor' CLI tool.
    // -b html5: Output HTML
    // -s: No header/footer (fragment mode)
    // -o -: Output to stdout

    // Note: We use the source file path directly.
    // This ensures that AsciiDoc 'include::' directives work relative to the
    // file.
    // We use -a showtitle to get the document title in the output.
    std::string cmd =
        std::format("asciidoctor -b html5 -s -a showtitle -o - \"{}\"",
                    ctx.source_path.string());

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
      return std::unexpected(
          "Failed to run 'asciidoctor'. Is it installed and in PATH?");
    }

    std::array<char, 256> buffer;
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
      result += buffer.data();
    }

    int returnCode = pclose(pipe);
    if (returnCode != 0) {
      return std::unexpected(std::format(
          "'asciidoctor' failed with exit code {}. Check console for errors.",
          returnCode));
    }

    // Extract <h1> title if present and move to metadata
    size_t h1_start = result.find("<h1>");
    size_t h1_end = result.find("</h1>");
    if (h1_start != std::string::npos && h1_end != std::string::npos &&
        h1_end > h1_start) {
      std::string title =
          result.substr(h1_start + 4, h1_end - (h1_start + 4));
      ctx.meta_data["title"] = title;
      // We keep the <h1> in the content so it renders in the main body.
      // If specific templates render {{ title }} themselves, they might duplicate it,
      // but for this generic template, we need it in the content.
    }

    return result;
  }
};

class AsciiDocPlugin : public core::IPlugin {
public:
  std::string name() const override { return "AsciiDoc Renderer"; }
  std::string version() const override { return "0.1.0"; }
  std::string description() const override {
    return "Renders .adoc files using 'asciidoctor' CLI";
  }

  void on_init(model::SiteContext &ctx) override {
    // Optional: Check if asciidoctor is available
    // std::system("asciidoctor --version > /dev/null 2>&1")
  }

  std::unique_ptr<core::IRenderer>
  create_renderer(const std::string &extension) override {
    if (extension == ".adoc" || extension == ".asciidoc") {
      return std::make_unique<AsciiDocRenderer>();
    }
    return nullptr;
  }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::AsciiDocPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }
