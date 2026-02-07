# Build Process and Dependencies

This document describes how to build the `gh_docs_bot` application from source and lists the necessary dependencies.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Prerequisites](#prerequisites)
- [Dependencies](#dependencies)
- [Configuration](#configuration)
- [Build Instructions](#build-instructions)
- [Output](#output)
- [Usage](#usage)
  - [Configuration](#configuration-1)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Prerequisites

Before building the project, ensure that your development environment meets the following requirements:

- **CMake**: Version **3.23** or higher.
- **C++ Compiler**: A compiler with **C++23** support (e.g., GCC 13+, Clang 16+, MSVC 19.36+).
- **Make** or **Ninja**: Build tool.
- **libcurl**: Development headers and library must be installed on your system.
  - _Ubuntu/Debian_: `sudo apt install libcurl4-openssl-dev`

## Dependencies

The project relies on the following libraries. Most are handled automatically by CMake via `FetchContent`.

| Dependency        | Type     | Source                         | Description                              |
| :---------------- | :------- | :----------------------------- | :--------------------------------------- |
| **nlohmann_json** | External | `FetchContent` (v3.11.2)       | JSON for Modern C++.                     |
| **md4c**          | External | `FetchContent` (release-0.5.2) | Markdown parser implementation.          |
| **CURL**          | System   | `find_package`                 | Library for transferring data with URLs. |
| **inja**          | Embedded | `include/inja.hpp`             | Template engine for Modern C++.          |

## Configuration

The build references a `configure` subdirectory which generates `rz_config.hpp` with project metadata (Version, Author, etc.).

## Build Instructions

Follow these steps to configure and build the application:

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/Zheng-Bote/gh-docs-bot.git
    cd gh-docs-bot
    ```

2.  **Configure the project:**

    ```bash
    cmake -S . -B build
    ```

3.  **Build the target:**

    ```bash
    cmake --build build -j$(nproc) --target gh_docs_bot
    ```

4.  **Install (Optional):**
    ```bash
    sudo cmake --install build
    ```

## Output

After a successful build, the executable `gh_docs_bot` will be available in the `build/` directory (or `build/Debug`/`build/Release` depending on the platform/generator).

## Usage

```bash
./build/gh_docs_bot <path/to/config.cfg> <path/to/documentation-folder>
```

### Configuration

**Example configuration file**

```Ini
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
