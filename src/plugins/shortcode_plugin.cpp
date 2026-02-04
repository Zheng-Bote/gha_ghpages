/**
 * SPDX-FileComment: Shortcode Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file shortcode_plugin.cpp
 * @brief Replaces shortcodes like [[ name: args ]] with HTML.
 * @version 0.1.0
 * @date 2026-02-04
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <functional>
#include <sstream>

namespace ssg::plugins {

class ShortcodePlugin : public core::IPlugin {
    using ShortcodeHandler = std::function<std::string(const std::map<std::string, std::string>&)>;
    std::map<std::string, ShortcodeHandler> handlers;

    // Helper to parse arguments string: "id=123, text='Hello World'" or just "123" (positional)
    std::map<std::string, std::string> parse_args(const std::string& args_str) {
        std::map<std::string, std::string> args;
        
        // Simple regex for key=value or value
        // Note: This is a basic parser. Complex quoting might need a better tokenizer.
        // We assume comma separation.
        std::stringstream ss(args_str);
        std::string segment;
        int positional_index = 0;

        while (std::getline(ss, segment, ',')) {
            // Trim whitespace
            auto start = segment.find_first_not_of(" \t");
            if (start == std::string::npos) continue;
            auto end = segment.find_last_not_of(" \t");
            segment = segment.substr(start, end - start + 1);

            size_t eq_pos = segment.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = segment.substr(0, eq_pos);
                std::string val = segment.substr(eq_pos + 1);
                
                // Remove quotes from val if present
                if (val.size() >= 2 && (val.front() == '"' || val.front() == '\'')) {
                    val = val.substr(1, val.size() - 2);
                }
                
                // Trim key
                auto k_end = key.find_last_not_of(" \t");
                if (k_end != std::string::npos) key = key.substr(0, k_end + 1);
                
                args[key] = val;
            } else {
                // Positional arg
                if (positional_index == 0) args["_0"] = segment; // Main arg
                else args["_"] = std::to_string(positional_index) + segment;
                positional_index++;
            }
        }
        return args;
    }

public:
    std::string name() const override { return "Shortcode Processor"; }
    std::string version() const override { return "1.0.0"; }
    std::string description() const override { return "Processes [[ shortcode ]] syntax"; }

    ShortcodePlugin() {
        // --- 1. YouTube ---
        // Syntax: [[ youtube: VIDEO_ID ]] or [[ youtube: id=VIDEO_ID ]]
        handlers["youtube"] = [](const auto& args) -> std::string {
            std::string id = args.count("id") ? args.at("id") : (args.count("_0") ? args.at("_0") : "");
            if (id.empty()) return "<!-- Missing YouTube ID -->";
            
            return std::format(R"(<div class="video-container"><iframe src="https://www.youtube.com/embed/{}" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe></div>)", id);
        };

        // --- 2. Alert / Note ---
        // Syntax: [[ alert: type=warning, text="Message" ]]
        // Positional: [[ alert: Message ]] -> defaults to info
        handlers["alert"] = [](const auto& args) -> std::string {
            std::string type = args.count("type") ? args.at("type") : "info";
            std::string text = args.count("text") ? args.at("text") : (args.count("_0") ? args.at("_0") : "");
            
            std::string class_name = "info-msg";
            if (type == "warning") class_name = "warning-msg";
            else if (type == "error" || type == "danger") class_name = "error-msg";
            else if (type == "success") class_name = "success-msg";

            return std::format(R"(<div class="{}">{}</div>)", class_name, text);
        };

        // --- 3. Button ---
        // Syntax: [[ button: url=..., text=..., alt=..., title=... ]]
        handlers["button"] = [](const auto& args) -> std::string {
            std::string url = args.count("url") ? args.at("url") : "#";
            std::string text = args.count("text") ? args.at("text") : "Click";
            std::string alt = args.count("alt") ? args.at("alt") : text;
            std::string title = args.count("title") ? args.at("title") : url;
            std::string type = args.count("type") ? args.at("type") : "primary"; // primary, outline, ghost

            return std::format(R"(<a href="{}" class="btn btn-{}" alt="{}" title="{}">{}</a>)", 
                url, type, alt, title, text);
        };
    }

    void on_before_render(model::PageContext& ctx) override {
        // Regex to find [[ name: args ]]
        // Group 1: name
        // Group 2: args
        std::regex re(R"(\[\[\s*([a-zA-Z0-9_-]+)\s*:\s*(.*?)\s*\]\])");
        
        std::string content = ctx.raw_content;
        std::string result;
        
        auto begin = std::sregex_iterator(content.begin(), content.end(), re);
        auto end = std::sregex_iterator();
        
        size_t last_pos = 0;
        
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            
            // Append text before match
            result.append(content, last_pos, match.position() - last_pos);
            
            std::string name = match[1].str();
            std::string args_str = match[2].str();
            
            if (handlers.contains(name)) {
                auto args = parse_args(args_str);
                result += handlers[name](args);
            } else {
                // Unknown shortcode, leave as is or show error?
                // Leaving as is allows future/other plugins or just text.
                result += match.str();
            }
            
            last_pos = match.position() + match.length();
        }
        
        // Append remaining text
        result.append(content, last_pos, std::string::npos);
        
        ctx.raw_content = result;
    }
};

} // namespace ssg::plugins

// --- Factory Functions ---

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::ShortcodePlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}
