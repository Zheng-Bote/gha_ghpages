/**
 * SPDX-FileComment: HTML Plugin Header
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file html_plugin.hpp
 * @brief Plugin that provides a simple HTML Pass-through Renderer.
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

class HtmlPlugin : public core::IPlugin {
public:
    std::string name() const override { return "HTML Pass-through"; }
    
    std::unique_ptr<core::IRenderer> create_renderer(const std::string& extension) override;
};

} // namespace ssg::plugins
