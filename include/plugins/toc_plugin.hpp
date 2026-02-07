/**
 * SPDX-FileComment: TOC Plugin Header
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file toc_plugin.hpp
 * @brief Plugin for generating Table of Contents and auto-ID headers.
 * @version 0.1.3
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#include "core/interfaces.hpp"
#include <set>
#include <sstream>

namespace ssg::plugins {

class TocPlugin : public core::IPlugin {
  struct State {
    bool in_header = false;
    int current_level = 0;
    std::stringstream buffer;
  } state;

  std::set<std::string> used_ids;

  std::string slugify(const std::string &text);

public:
  std::string name() const override { return "TOC & Auto-ID Generator"; }
  std::string version() const override { return "0.1.3"; }
  std::string description() const override {
    return "Generates Table of Contents and auto-ID headers";
  }

  void on_init(model::SiteContext &ctx) override;
  void on_before_render(model::PageContext &ctx) override;

  // MD4C Hooks
  bool on_md_block_enter(MD_BLOCKTYPE type, void *detail,
                         model::PageContext &ctx, std::ostream &out) override;
  bool on_md_block_leave(MD_BLOCKTYPE type, void *detail,
                         model::PageContext &ctx, std::ostream &out) override;
  bool on_md_text(MD_TEXTTYPE type, const char *text, MD_SIZE size,
                  model::PageContext &ctx, std::ostream &out) override;

  // Injection
  void on_after_render(model::PageContext &ctx) override;
};

} // namespace ssg::plugins
