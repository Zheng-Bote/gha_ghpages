/**
 * SPDX-FileComment: Office Plugin Header
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file office_plugin.hpp
 * @brief Plugin for parsing Office documents (ODT, DOCX) to HTML.
 * @version 0.1.0
 * @date 2026-02-06
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#include "core/interfaces.hpp"

namespace ssg::plugins {

class OfficePlugin : public core::IPlugin {
    core::IPluginHost& host;

public:
    explicit OfficePlugin(core::IPluginHost& h) : host(h) {}

    std::string name() const override { return "office"; }
    std::string version() const override { return "0.1.0"; }
    std::string description() const override { return "Parses ODT, DOCX files to HTML."; }

    std::unique_ptr<core::IRenderer> create_renderer(const std::string& extension) override;
};

} // namespace ssg::plugins
