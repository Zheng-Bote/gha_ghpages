/**
 * SPDX-FileComment: HTML Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file html_plugin.cpp
 * @brief Implementation of the HTML Plugin.
 * @version 0.1.1
 * @date 2026-02-06
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "plugins/html_plugin.hpp"
#include <iostream>

namespace ssg::plugins {

class HtmlRenderer : public core::IRenderer {
public:
    std::expected<std::string, std::string> render(model::PageContext& ctx) override {
        // Pass-through: Just return the raw content (which is HTML)
        return ctx.raw_content;
    }
};

std::unique_ptr<core::IRenderer> HtmlPlugin::create_renderer(const std::string& extension) {
    if (extension == ".htm" || extension == ".html") {
        return std::make_unique<HtmlRenderer>();
    }
    return nullptr;
}

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::HtmlPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}