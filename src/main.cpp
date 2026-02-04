/**
 * SPDX-FileComment: Main Entry Point
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file main.cpp
 * @brief Entry point for the SSG.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "core/site_builder.hpp"
#include <print>
#include <iostream>

void print_usage(const char* prog) {
    std::println("Usage: {} <config_file> <input_dir>", prog);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    try {
        ssg::core::SiteBuilder builder(argv[1]);
        builder.set_source_root(argv[2]);
        
        builder.register_plugins();
        builder.build();
        
    } catch (const std::exception& e) {
        std::println(stderr, "Fatal Error: {}", e.what());
        return 1;
    }

    return 0;
}
