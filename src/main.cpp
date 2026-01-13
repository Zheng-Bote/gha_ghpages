/*
 * SPDX-License-Identifier: MIT
 * Author: Robert Zheng
 * Copyright (c) 2026 ZHENG Robert
 * Version: 1.6.0
 * Description: Static Site Generator (Version 10 - Hide Root Files in Nav)
 */

/**
 * Compile:
 * g++ -std=c++23 -o ssg main11.cpp -lmd4c-html -lmd4c
 */

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

// Libraries
#include <inja.hpp>
#include <md4c-html.h>
#include <nlohmann/json.hpp>

// User config header
#include <rz_config.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

// --- Structures ---

struct Config {
  fs::path templatePath;
  fs::path outputDir = "output_site";
  fs::path assetsPath;
};

struct DirNode {
  fs::path relativePath;        ///< Path relative to root.
  std::string dirName;          ///< Name of the directory.
  std::vector<fs::path> files;  ///< List of files in this directory.
  std::vector<DirNode> subdirs; ///< List of subdirectories.
};

// --- Helpers ---

std::string readFile(const fs::path &path) {
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (!in)
    throw std::runtime_error(
        std::format("Could not read file: {}", path.string()));
  std::string content((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());
  return content;
}

void writeFile(const fs::path &path, std::string_view content) {
  std::ofstream out(path, std::ios::out | std::ios::binary);
  if (!out)
    throw std::runtime_error(
        std::format("Could not write file: {}", path.string()));
  out << content;
}

// --- Asset Management ---

void copyAssets(const fs::path &sourceAssets, const fs::path &outputRoot) {
  fs::path destAssets = outputRoot / "assets";

  if (fs::exists(sourceAssets) && fs::is_directory(sourceAssets)) {
    std::cout << "Copying assets from: " << sourceAssets.string() << std::endl;
    try {
      fs::create_directories(destAssets);
      fs::copy(sourceAssets, destAssets,
               fs::copy_options::recursive |
                   fs::copy_options::overwrite_existing);
      std::cout << "Assets successfully copied." << std::endl;
    } catch (const fs::filesystem_error &e) {
      std::cerr << "Error copying assets: " << e.what() << std::endl;
    }
  } else {
    if (!sourceAssets.empty()) {
      std::cerr << "Warning: Assets path configured but not found: "
                << sourceAssets.string() << std::endl;
    }
  }
}

// --- Markdown Logic ---

void md_process_output(const MD_CHAR *text, MD_SIZE size, void *userdata) {
  std::string *out = static_cast<std::string *>(userdata);
  out->append(text, size);
}

std::string renderMarkdown(const std::string &mdContent) {
  std::string htmlOutput;
  int ret = md_html(mdContent.c_str(), static_cast<MD_SIZE>(mdContent.size()),
                    md_process_output, &htmlOutput, MD_DIALECT_GITHUB, 0);
  if (ret != 0)
    throw std::runtime_error("Markdown parsing failed.");
  return htmlOutput;
}

// --- Config Parser ---

Config parseConfig(const fs::path &configPath) {
  Config cfg;
  std::ifstream file(configPath);
  if (!file)
    throw std::runtime_error("Configuration file not found.");
  std::string line;
  while (std::getline(file, line)) {
    auto delimiterPos = line.find('=');
    if (delimiterPos != std::string::npos) {
      std::string key = line.substr(0, delimiterPos);
      std::string value = line.substr(delimiterPos + 1);

      if (!value.empty() && value.back() == '\r')
        value.pop_back();

      if (key == "template")
        cfg.templatePath = value;
      else if (key == "output")
        cfg.outputDir = value;
      else if (key == "assets")
        cfg.assetsPath = value;
    }
  }
  return cfg;
}

// --- Logic: Build Tree ---

bool isSupportedFile(const fs::path &p) {
  std::string ext = p.extension().string();
  return (ext == ".md" || ext == ".htm");
}

DirNode buildTree(const fs::path &currentPath, const fs::path &rootPath) {
  DirNode node;
  node.dirName = currentPath.filename().string();

  node.relativePath = fs::relative(currentPath, rootPath);
  if (node.relativePath == ".")
    node.relativePath = "";

  for (const auto &entry : fs::directory_iterator(currentPath)) {
    std::string name = entry.path().filename().string();

    // Skip hidden files/folders
    if (name.starts_with("."))
      continue;

    if (entry.is_directory()) {
      // Skip assets folder in nav tree
      if (name == "assets")
        continue;

      node.subdirs.push_back(buildTree(entry.path(), rootPath));
    } else if (entry.is_regular_file()) {
      if (isSupportedFile(entry.path())) {
        node.files.push_back(entry.path().filename());
      }
    }
  }
  std::ranges::sort(node.subdirs, {}, &DirNode::dirName);
  std::ranges::sort(node.files);
  return node;
}

std::string getBackPrefix(const fs::path &currentRelPath) {
  std::string prefix = "";
  for (const auto &_ : currentRelPath) {
    prefix += "../";
  }
  return prefix;
}

fs::path getTargetFilename(const fs::path &sourceFile) {
  fs::path p = sourceFile;
  std::string ext = p.extension().string();

  if (ext == ".md" || ext == ".htm") {
    p.replace_extension(".html");
  }
  return p;
}

// --- Navigation Generator ---

/**
 * @brief Generates Navigation HTML.
 * New logic: If isRoot is true, files in that node are NOT rendered.
 */
void generateNavHtml(const DirNode &currentNode, std::string &html,
                     const std::string &urlPrefix,
                     const fs::path &activeTargetFile,
                     bool isRoot) { // <--- Added Parameter

  html += "<ul class=\"nav-list\">\n";

  // 1. Files (ONLY if not root)
  if (!isRoot) {
    for (const auto &file : currentNode.files) {
      std::string nameNoExt = file.stem().string();
      fs::path targetFile = getTargetFilename(file);
      fs::path fullLinkPath = currentNode.relativePath / targetFile;

      std::string href = urlPrefix + fullLinkPath.generic_string();

      std::string classAttr =
          (fullLinkPath == activeTargetFile) ? " class=\"active\"" : "";

      html += std::format("  <li><a href=\"{}\"{}>{}</a></li>\n", href,
                          classAttr, nameNoExt);
    }
  }

  // 2. Subdirectories (Always rendered)
  for (const auto &sub : currentNode.subdirs) {
    html += std::format("  <li><strong>{}</strong>\n", sub.dirName);

    // Recursive call: subfolders are never "root", so pass false
    generateNavHtml(sub, html, urlPrefix, activeTargetFile, false);

    html += "  </li>\n";
  }
  html += "</ul>\n";
}

// --- Processing ---

void processFiles(const DirNode &currentNode, const DirNode &rootNode,
                  const fs::path &inputRoot, const Config &cfg,
                  inja::Environment &env, const inja::Template &tmpl) {

  fs::path currentOutputDir = cfg.outputDir / currentNode.relativePath;
  fs::create_directories(currentOutputDir);
  std::string backPrefix = getBackPrefix(currentNode.relativePath);

  for (const auto &file : currentNode.files) {
    fs::path inputPath = inputRoot / currentNode.relativePath / file;
    fs::path targetFilename = getTargetFilename(file);
    fs::path outputPath = currentOutputDir / targetFilename;
    fs::path currentActiveFile = currentNode.relativePath / targetFilename;

    std::string navHtml;
    // Pass 'true' because rootNode represents the root directory
    generateNavHtml(rootNode, navHtml, backPrefix, currentActiveFile, true);

    std::string rawContent = readFile(inputPath);
    std::string contentToInject;
    std::string ext = file.extension().string();

    if (ext == ".md") {
      contentToInject = renderMarkdown(rawContent);
    } else if (ext == ".htm") {
      contentToInject = rawContent;
    }

    json data;
    data["base_path"] = backPrefix;
    data["title"] = file.stem().string();
    data["navigation"] = navHtml;
    data["content"] = contentToInject;

    try {
      std::string finalResult = env.render(tmpl, data);
      writeFile(outputPath, finalResult);
      std::cout << "Created: " << outputPath.string() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Template Error in " << file.string() << ": " << e.what()
                << std::endl;
    }
  }

  for (const auto &sub : currentNode.subdirs) {
    processFiles(sub, rootNode, inputRoot, cfg, env, tmpl);
  }
}

void printUsage(char *argv[]) {
  std::cout << rz::config::PROJECT_NAME.data() << " v"
            << rz::config::VERSION.data() << "\n";
  std::cerr << "Usage: " << argv[0] << " <path_to_config> <input_folder>"
            << std::endl;
}

// --- Main ---

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printUsage(argv);
    return 1;
  }

  fs::path configPath = argv[1];
  fs::path inputDir = argv[2];

  try {
    Config cfg = parseConfig(configPath);

    if (!fs::exists(inputDir))
      throw std::runtime_error("Input folder does not exist.");
    if (!fs::exists(cfg.templatePath))
      throw std::runtime_error("Template file does not exist.");

    std::cout << "Scanning structure (.md & .htm only)..." << std::endl;
    DirNode rootNode = buildTree(inputDir, inputDir);

    if (fs::exists(cfg.outputDir))
      fs::remove_all(cfg.outputDir);
    fs::create_directories(cfg.outputDir);

    if (!cfg.assetsPath.empty()) {
      copyAssets(cfg.assetsPath, cfg.outputDir);
    }

    std::cout << "Loading template..." << std::endl;
    inja::Environment env;
    inja::Template tmpl = env.parse_template(cfg.templatePath.string());

    std::cout << "Generating pages..." << std::endl;
    processFiles(rootNode, rootNode, inputDir, cfg, env, tmpl);

    std::cout << "Done! Output in: " << cfg.outputDir.string() << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}