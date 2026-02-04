/**
 * SPDX-FileComment: Markdown Plugin Header
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file markdown_plugin.hpp
 * @brief Plugin that provides a Markdown Renderer using md4c.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#include "core/interfaces.hpp"

namespace ssg::plugins {

class MarkdownPlugin : public core::IPlugin {
    core::IPluginHost& host;

public:
    explicit MarkdownPlugin(core::IPluginHost& h) : host(h) {}

    std::string name() const override { return "Markdown Renderer"; }
    std::string version() const override { return "1.1.0"; }
    std::string description() const override { return "Renders Markdown files using md4c"; }
    
    // Provides the renderer
    std::unique_ptr<core::IRenderer> create_renderer(const std::string& extension) override;
};

} // namespace ssg::plugins
