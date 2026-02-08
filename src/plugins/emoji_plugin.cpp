/**
 * SPDX-FileComment: Emoji Shortcode Plugin
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file emoji_plugin.cpp
 * @brief Replaces :shortcode: with HTML entities or UTF-8 chars.
 * @version 0.1.1
 * @date 2026-02-07
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/interfaces.hpp"
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

namespace ssg::plugins {

class EmojiPlugin : public core::IPlugin {
  std::unordered_map<std::string, std::string> emoji_map;

public:
  std::string name() const override { return "Emoji Shortcodes"; }
  std::string version() const override { return "0.1.1"; }
  std::string description() const override {
    return "Replaces :shortcode: with HTML entities";
  }

  void on_init(model::SiteContext &ctx) override {
    // --- GitHub Alert Semantics ---
    // Mapping based on standard GitHub Alert icons

    // > [!NOTE] -> Blue Info Icon
    emoji_map["note"] = "&#8505;"; // ℹ️
    emoji_map["info"] = "&#8505;"; // ℹ️

    // > [!TIP] -> Light Bulb
    emoji_map["tip"] = "&#128161;";  // 💡
    emoji_map["hint"] = "&#128161;"; // 💡
    emoji_map["bulb"] = "&#128161;"; // 💡

    // > [!IMPORTANT] -> Purple/Notification (using Heavy Exclamation or
    // Loudspeaker)
    emoji_map["important"] = "&#10071;"; // ❗
    emoji_map["attention"] = "&#10071;"; // ❗

    // > [!WARNING] -> Yellow Triangle
    emoji_map["warning"] = "&#9888;"; // ⚠️
    emoji_map["warn"] = "&#9888;";    // ⚠️

    // > [!CAUTION] -> Red Stop/Octagon
    emoji_map["caution"] = "&#128721;"; // 🛑
    emoji_map["danger"] = "&#128721;";  // 🛑
    emoji_map["stop"] = "&#128721;";    // 🛑

    // --- Standard Sets ---

    // General
    emoji_map["check"] = "&#10004;"; // ✔
    emoji_map["cross"] = "&#10060;"; // ❌
    emoji_map["x"] = "&#10060;";     // ❌

    // Faces
    emoji_map["smile"] = "&#128512;";      // 😀
    emoji_map["laugh"] = "&#128514;";      // 😂
    emoji_map["wink"] = "&#128521;";       // 😉
    emoji_map["cool"] = "&#128526;";       // 😎
    emoji_map["thinking"] = "&#129300;";   // 🤔
    emoji_map["sad"] = "&#128546;";        // 😢
    emoji_map["silhouette"] = "&#128100;"; // �

    // Hands
    emoji_map["thumbsup"] = "&#128077;";      // 👍
    emoji_map["thumbsdown"] = "&#128078;";    // 👎
    emoji_map["ok_hand"] = "&#128076;";       // 👌
    emoji_map["clap"] = "&#128079;";          // 👏
    emoji_map["handshake"] = "&#129309;";     // 🤝
    emoji_map["vulcan_salute"] = "&#128406;"; // 🖖

    // Arrows
    emoji_map["arrow_right"] = "&#10145;"; // ➡
    emoji_map["arrow_left"] = "&#11013;";  // ⬅
    emoji_map["arrow_up"] = "&#11014;";    // ⬆
    emoji_map["arrow_down"] = "&#11015;";  // ⬇

    // Objects / Misc
    emoji_map["rocket"] = "&#128640;";                // 🚀
    emoji_map["fire"] = "&#128293;";                  // 🔥
    emoji_map["star"] = "&#11088;";                   // ⭐
    emoji_map["heart"] = "&#10084;";                  // ❤
    emoji_map["chilli"] = "&#127788;";                // 🌶️
    emoji_map["beer"] = "&#127866;";                  // 🍺
    emoji_map["coffee"] = "&#9749;";                  // ☕
    emoji_map["book"] = "&#128218;";                  // 📖
    emoji_map["building_construction"] = "&#127959;"; // 🚧
    emoji_map["classical_building"] = "&#127968;";    // 🏛️
    emoji_map["email"] = "&#9993;";                   // ✉️
    emoji_map["phone"] = "&#128222;";                 // 📞
    emoji_map["camera"] = "&#128247;";                // 📸
    emoji_map["key"] = "&#128272;";                   // 🔑
    emoji_map["lock"] = "&#128274;";                  // 🔒
    emoji_map["unlock"] = "&#128275;";                // 🔓
    emoji_map["wrench"] = "&#128295;";                // 🔧
    emoji_map["pushpin"] = "&#128205;";               // 📌
    emoji_map["round_pushpin"] = "&#128206;";         // 📍
    emoji_map["paperclip"] = "&#128204;";             // 📎
    emoji_map["folder"] = "&#128199;";                // 📁
    emoji_map["open_file_folder"] = "&#128200;";      // 📂
    emoji_map["hammer_and_pick"] = "&#9874;";         // ⛏️

    // Animals
    emoji_map["penguin"] = "&#129443;";     // 🐧
    emoji_map["rabbit_face"] = "&#128048;"; // 🐇
    emoji_map["panda_face"] = "&#128062;";  // 🐼
  }

  void on_after_render(model::PageContext &ctx) override {
    // Regex fängt :wort_123:
    std::regex re(":([a-z0-9_+-]+):");

    std::string new_content;
    new_content.reserve(ctx.html_content.size());

    auto words_begin = std::sregex_iterator(ctx.html_content.begin(),
                                            ctx.html_content.end(), re);
    auto words_end = std::sregex_iterator();

    size_t last_pos = 0;

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;

      // Text VOR dem Match anhängen
      new_content.append(ctx.html_content, last_pos,
                         match.position() - last_pos);

      std::string key = match[1].str(); // Der Text innerhalb der Doppelpunkte

      // Check ob Key in Map existiert
      if (emoji_map.contains(key)) {
        new_content.append(emoji_map[key]);
      } else {
        // Wenn nicht bekannt, lassen wir es unverändert
        new_content.append(match[0].str());
      }

      last_pos = match.position() + match.length();
    }

    // Rest des Strings anhängen
    new_content.append(ctx.html_content, last_pos, std::string::npos);

    ctx.html_content = new_content;
  }
};

} // namespace ssg::plugins

extern "C" ssg::core::IPlugin *create_plugin(ssg::core::IPluginHost *host) {
  return new ssg::plugins::EmojiPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin *plugin) { delete plugin; }