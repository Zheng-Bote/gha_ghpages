# Github Action for GitHub Pages - gh_docs_bot

![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Github_Action_|_Linux-lightgrey.svg)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/Zheng-Bote/gha_ghpages?logo=GitHub)](https://github.com/Zheng-Bote/gha_ghpages/releases)

**gh_docs_bot** is a high-performance C++ Static Site Generator (SSG) built as a reusable GitHub Action. It is designed to automatically build and deploy static documentation websites from Markdown, HTML, AsciiDoc, and Office files (ODT/DOCX) directly within your GitHub workflows.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

**Table of Contents**

- [Github Action for GitHub Pages - gh_docs_bot](#github-action-for-github-pages---gh_docs_bot)
- [Description](#description)
  - [🚀 Features](#-features)
  - [Included Plugins](#included-plugins)
  - [Status](#status)
  - [📦 Usage](#-usage)
    - [Quick Start](#quick-start)
  - [📚 Documentation](#-documentation)
    - [Screenshoots of included themes](#screenshoots-of-included-themes)
- [📄 License](#-license)
- [🤝 Contributing](#-contributing)
- [👤 Author](#-author)
  - [Code Contributors](#code-contributors)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

# Description

Github Action for building and deploying static documentation websites directly within your GitHub workflows.

A lightweight, high-performance Static Site Generator that converts a directory of content files into a static HTML website. It features recursive directory scanning, automatic navigation generation, modern templating, and automated asset management.

## 🚀 Features

- **High Performance**: Built with **C++23**, utilizing `md4c` for ultra-fast Markdown processing.
- **Extensible Plugin System**: Modular architecture allowing dynamic content generation. Includes already a set of powerful built-in plugins
- **Zero-Config Bundle**: The GitHub Action handles all dependencies, including **LLVM 21**, **CMake**, and **Ninja**.
- **Customizable**: Uses [Inja](https://github.com/pantor/inja) for flexible HTML templating.
- **Multi-Format Support**: Supports Markdown, HTML, AsciiDoc, ODT, and DOCX files.
- **GH-Pages Templates**: Includes a set of beautiful and responsive templates for GitHub Pages.
- **GitHub Pages Ready**: Optimized for deploying directly to `gh-pages`.
- **Security Focused**: Includes built-in SBOM generation and security slices (via CDXGen/Atom integration in the repo).

## Included Plugins

| Name                        | Description                                                 |
| :-------------------------- | :---------------------------------------------------------- |
| **AsciiDoc Renderer**       | Renders `.adoc` files using `asciidoctor` CLI               |
| **Breadcrumb Navigation**   | Generates breadcrumb HTML with folder links                 |
| **Emoji Shortcodes**        | Replaces `shortcode` with HTML entities                     |
| **HTML Renderer**           | Renders HTML files                                          |
| **Info & Alerts**           | Processes GFM-style alerts and specific emojis              |
| **Markdown Renderer**       | Renders Markdown files using `md4c`                         |
| **Mermaid Diagram Support** | Renders `mermaid` code blocks as diagrams                   |
| **Minifier**                | Minifies HTML and CSS output                                |
| **office**                  | Parses ODT, DOCX files to HTML                              |
| **Reading Time Calculator** | Estimates reading time for long content                     |
| **Search Index Generator**  | Generates `search.json` for client-side search              |
| **Shortcode Processor**     | Processes `shortcode` syntax                                |
| **Sitemap Generator**       | Generates `sitemap.xml` and folder-based `index.html` files |
| **TOC & Auto-ID Generator** | Generates Table of Contents and auto-ID headers             |

## Status

[![C++ Quality & Security](https://github.com/Zheng-Bote/gha_ghpages/actions/workflows/quality_security.yml/badge.svg)](https://github.com/Zheng-Bote/gha_ghpages/actions/workflows/quality_security.yml)
[![Release SBOM](https://github.com/Zheng-Bote/gha_ghpages/actions/workflows/sbom.yml/badge.svg)](https://github.com/Zheng-Bote/gha_ghpages/actions/workflows/sbom.yml)

## 📦 Usage

You can use this action in your own workflows to build your documentation site. It will compile the generator from source (ensuring the latest version) and run it against your documentation folder.

### Quick Start

Add the following step to your `.github/workflows/deploy.yml`:

```yaml
- name: Build and Generate Site
  uses: Zheng-Bote/gha_ghpages@main
  with:
    template: "theme/desktop/template.html" # Path to your HTML template
    assets: "theme/tron/assets" # Path to your assets
    output: "public" # Output directory
    docs: "docs" # Directory with Markdown (*.md) and/or HTM (*.htm) files
    base_path: "https://zheng-bote.github.io/gha_ghpages/" # your GitHub Pages URL
    toc: "true" # Generate Table of Contents
    plugins: "docs/plugins" # Path to your plugins
```

> \[!TIP]
> _see themes folder for examples_

## 📚 Documentation

For a comprehensive guide on how to configure inputs, set up your repository structure, and a full workflow example, please read our **[How-To Guide](docs/howtos/index.md)** or the **[GitHub Pages: Documentation](https://zheng-bote.github.io/gha_ghpages/)**.

### Screenshoots of included themes

![Themes](theme/fluid/assets/img_tmp/themes_2.png)

> \[!NOTE]
> _see more screenshoots in_ [GitHub Pages: Documentation](https://zheng-bote.github.io/gha_ghpages/)

---

([back to top](#top))

# 📄 License

Distributed under the MIT License. See LICENSE for more information.

Copyright (c) 2026 ZHENG Robert

# 🤝 Contributing

Contributions are welcome! Please fork the repository and create a pull request.

1. Fork the Project
2. Create your Feature Branch (git checkout -b feature/AmazingFeature)
3. Commit your Changes (git commit -m 'Add some AmazingFeature')
4. Push to the Branch (git push origin feature/AmazingFeature)
5. Open a Pull Request

# 👤 Author

[![Zheng Robert - Core Development](https://img.shields.io/badge/Github-Zheng_Robert-black?logo=github)](https://www.github.com/Zheng-Bote)

## Code Contributors

![Contributors](https://img.shields.io/github/contributors/Zheng-Bote/gha_ghpages?color=dark-green)

---

([back to top](#top))

:vulcan_salute:
