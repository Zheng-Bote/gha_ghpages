# Github Action for GitHub Pages - gh_docs_bot

![License](https://img.shields.io/badge/license-MIT-green.svg)

**gh_docs_bot** is a high-performance, custom C++ Static Site Generator (SSG) packaged as a reusable GitHub Action. It is designed to automatically build and deploy static documentation websites from Markdown files directly within your GitHub workflows.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

**Table of Contents**

autocreated by doctoc

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

# Description

Github Action for building and deploying static documentation websites from Markdown files directly within your GitHub workflows.

A lightweight, high-performance Static Site Generator that converts a directory of Markdown files into a static HTML website. It features recursive directory scanning, automatic navigation generation, modern templating and automated asset management.

## 🚀 Features

- **High Performance**: Built with **C++23**, utilizing `md4c` for ultra-fast Markdown processing.
- **Zero-Config Bundle**: The GitHub Action handles all dependencies, including **LLVM 21**, **CMake**, and **Ninja**.
- **Customizable**: Uses [Inja](https://github.com/pantor/inja) for flexible HTML templating.
- **GitHub Pages Ready**: Optimized for deploying directly to `gh-pages`.
- **Security Focused**: Includes built-in SBOM generation and security slices (via CDXGen/Atom integration in the repo).

## 📦 Usage

You can use this action in your own workflows to build your documentation site. It will compile the generator from source (ensuring the latest version) and run it against your documentation folder.

### Quick Start

Add the following step to your `.github/workflows/deploy.yml`:

```yaml
- name: Build and Generate Site
  uses: Zheng-Bote/gha_ghpages@main
  with:
    template: "theme/template.html" # Path to your HTML template
    output: "public" # Output directory
    docs: "docs" # Directory with Markdown files
```

## 📚 Documentation

For a comprehensive guide on how to configure inputs, set up your repository structure, and a full workflow example, please read our **[How-To Guide](docs/howtos/index.md)**.

## 🛠 Prerequisites

If you are running this locally or contributing, ensure you have:

- Clang 21+
- CMake & Ninja
- `libcurl`, `nlohmann-json`

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
