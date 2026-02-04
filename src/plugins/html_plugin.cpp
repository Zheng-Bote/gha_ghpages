/**
 * SPDX-FileComment: HTML Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file html_plugin.cpp
 * @brief Implementation of the HTML Plugin.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "plugins/html_plugin.hpp"
#include <regex>
#include <iostream>
#include <inja.hpp>

namespace ssg::plugins {

class HtmlRenderer : public core::IRenderer {
public:
    std::expected<std::string, std::string> render(model::PageContext& ctx) override {
        // ... (Header extraction logic)
        std::regex header_regex(R"(<h([1-6])([^>]*)>(.*?)</h\1>)", std::regex_constants::icase);
        std::regex id_regex(R"(id=["']([^"']*)["'])", std::regex_constants::icase);
        
        auto words_begin = std::sregex_iterator(ctx.raw_content.begin(), ctx.raw_content.end(), header_regex);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            int level = std::stoi(match[1].str());
            std::string attrs = match[2].str();
            std::string text = match[3].str();
            if (level >= 2 && level <= 4) {
                std::string id;
                std::smatch id_match;
                if (std::regex_search(attrs, id_match, id_regex)) {
                    id = id_match[1].str();
                } else {
                    id = "section-" + std::to_string(ctx.headers.size()); 
                }
                ctx.headers.push_back({level, text, id});
            }
        }

        // Pass-through: Just return the raw content (which is HTML)
        return ctx.raw_content;
    }
};

// ... (previous content)

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

