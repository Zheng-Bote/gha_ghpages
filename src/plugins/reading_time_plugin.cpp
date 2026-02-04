/**
 * SPDX-FileComment: Reading Time Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file reading_time_plugin.cpp
 * @brief Calculates reading time for pages exceeding a word count threshold.
 * @version 0.1.0
 * @date 2026-02-04
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cctype>

namespace ssg::plugins {

class ReadingTimePlugin : public core::IPlugin {
    int words_per_minute = 200;
    int min_words_threshold = 200;

public:
    std::string name() const override { return "Reading Time Calculator"; }
    std::string version() const override { return "1.0.0"; }
    std::string description() const override { return "Estimates reading time for long content"; }

    void on_init(model::SiteContext& ctx) override {
        // Read config if available
        if (ctx.config.contains("reading_time")) {
            auto& cfg = ctx.config["reading_time"];
            if (cfg.contains("wpm")) {
                words_per_minute = std::stoi(cfg["wpm"].get<std::string>());
            }
            if (cfg.contains("threshold")) {
                min_words_threshold = std::stoi(cfg["threshold"].get<std::string>());
            }
        }
    }

    void on_before_render(model::PageContext& ctx) override {
        // Count words in raw content
        // Simple heuristic: count whitespace transitions
        int words = 0;
        bool in_word = false;
        for (char c : ctx.raw_content) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                in_word = false;
            } else if (!in_word) {
                in_word = true;
                words++;
            }
        }

        ctx.meta_data["word_count"] = words;

        if (words >= min_words_threshold) {
            int minutes = std::max(1, (int)std::ceil((double)words / words_per_minute));
            ctx.meta_data["reading_time"] = std::to_string(minutes) + " min read";
            ctx.meta_data["show_reading_time"] = true;
        } else {
            ctx.meta_data["show_reading_time"] = false;
            ctx.meta_data["reading_time"] = "";
        }
    }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::ReadingTimePlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}
