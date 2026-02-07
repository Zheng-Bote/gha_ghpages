# User Guide

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [1. Prerequisites](#1-prerequisites)
- [2. Installation / Building](#2-installation--building)
- [3. Usage](#3-usage)
  - [Example](#example)
- [4. Configuration (`config.cfg`)](#4-configuration-configcfg)
- [5. Content Features](#5-content-features)
  - [5.1 Frontmatter / Metadata](#51-frontmatter--metadata)
  - [5.2 Table of Contents (TOC)](#52-table-of-contents-toc)
  - [5.3 Navigation](#53-navigation)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 1. Prerequisites

- **OS:** Linux (tested), Windows/macOS (should work).
- **Compiler:** C++23 compliant (GCC 13+, Clang 16+, MSVC latest).
- **Build System:** CMake 3.23+.

## 2. Installation / Building

1.  Clone the repository.
2.  Create a build directory:
    ```bash
    cmake -S . -B build
    ```
3.  Compile the project:
    ```bash
    cmake --build build -j$(nproc)
    ```
4.  The executable `gh_docs_bot` will be in `build/`.

## 3. Usage

The generic usage pattern is:

```bash
./build/gh_docs_bot <config_file> <input_directory>
```

### Example

```bash
./build/gh_docs_bot config.cfg docs/
```

## 4. Configuration (`config.cfg`)

The configuration file is a simple key-value pair file.

```ini
[General]
template=docs/theme/desktop/fluid_template.html
assets=docs/theme/fluid/assets
output=docs/dist
base_path=https://zheng-bote.github.io/gha_ghpages/
toc=true

[plugins]
path=docs/ssg/plugins
enabled=markdown, html, toc, asciidoc, search, sitemap, mermaid, breadcrumb, reading_time, shortcode, minify, office, info, emoji

[reading_time]
threshold=200
wpm=200
```

## 5. Content Features

### 5.1 Frontmatter / Metadata

Currently, the SSG treats the filename as the title.

- `file_name.md` -> Title: "file name"

### 5.2 Table of Contents (TOC)

The TOC is automatically generated for headers `H1` to `H4`.
It is injected into the HTML template variable `{ toc }}`.

If you are using Legacy templates, it also replaces the comment markers:

```html
<!-- START doctoc generated TOC ... -->
...
<!-- END doctoc generated TOC ... -->
```

### 5.3 Navigation

Navigation is generated based on the directory structure. Folders become categories, files become links.
