/**
 * SPDX-FileComment: Site and Page Context Definitions
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file context.hpp
 * @brief Defines the data structures for site and page contexts.
 * @version 0.1.0
 * @date 2026-02-03
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <any>
#include <nlohmann/json.hpp>

namespace ssg::model {

namespace fs = std::filesystem;
using json = nlohmann::json;

/**
 * @brief Represents a header found in the content (for TOC).
 */
struct Header {
    int level;
    std::string text;
    std::string id;
};

/**
 * @brief Context for a single page being processed.
 */
struct PageContext {
    fs::path source_path;
    fs::path output_path;
    fs::path relative_path; // Relative to source root
    
    std::string raw_content;
    std::string html_content;
    
    json meta_data; // Metadata for template (title, date, etc.)
    
    std::vector<Header> headers; // Collected headers for TOC
    
    // Allow plugins to store temporary page-local data
    std::map<std::string, std::any> plugin_data;
};

/**
 * @brief Global context for the entire site generation process.
 */
struct SiteContext {
    fs::path source_root;
    fs::path output_root;
    fs::path template_path;
    fs::path assets_path;
    
    json config; // Global configuration
    
    // Global shared data (e.g., all generated IDs for collision checking)
    std::map<std::string, std::any> shared_data;
};

} // namespace ssg::model
