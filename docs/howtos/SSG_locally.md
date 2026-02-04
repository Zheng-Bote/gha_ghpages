# SSG manually / locally

The gh_docs_bot is a custom C++ Static Site Generator (SSG) that allows you to build and generate a static website from your Markdown and HTML documentation. It encapsulates the entire build process, including dependency installation (LLVM 21, etc.), CMake configuration, and execution.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Usage](#usage)
  - [Inputs](#inputs)
- [Example](#example)
  - [Project Structure](#project-structure)
  - [Configuration](#configuration)
  - [Execution](#execution)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Usage

`Usage: build/gh_docs_bot <path_to_config> <input_folder>`

**_Example_**

`./gh_docs_bot /home/zb_bamboo/DEV/gh-a_cr-ghp/theme/blue/blue.cfg /home/zb_bamboo/DEV/gh-a_cr-ghp/docs`

### Inputs

| Input            | Description                                      |
| :--------------- | :----------------------------------------------- |
| `path_to_config` | Path to the config file to use for the site.     |
| `path_to_docs`   | Path to the input directory to use for the site. |

\

## Example

### Project Structure

```tree
my_project/
|__docs/
|   |__input_documents/
|   |  |__architecture/
|   |  |  |__overview.md
|   |  |  |__components.htm
|   |  |__howtos/
|   |     |__static_website.htm
|   |     |__github_action.md
|   |__themes/
|      |__desktop/
|      |  |__fluid_template.html
|      |  |__legacy_template.html
|      |__fluid/
|         |__stayle/
|         |  |__config.cfg
|         |  |__assets/
|         |     |__css/
|         |     |  |__fluid.css
|         |     |  |__layout.css
|         |     |  |__tokens.css
|         |     |__js/
|         |     |  |__action.js
|         |     |__imgs/
|         |     |  |__logo.png
|         |     |  |__favicon.ico
|         |__amber/
|         |  |__config.cfg
|         |     |__css/
|         |     |  |__fluid.css
|         |     |  |__layout.css
|         |     |  |__tokens.css
|         |     |__js/
|         |     |  |__action.js
|         |     |__imgs/
|         |     |  |__logo.png
|         |     |  |__favicon.ico
|__dist/
   |__website/
|     |__architecture/
|     |  |__overview.html
|     |  |__components.html
|     |__howtos/
|     |  |__static_website.html
|     |  |__github_action.html
|     |__assets/
|        |__css/
|        |  |__fluid.css
|        |  |__layout.css
|        |  |__tokens.css
|        |__js/
|        |  |__action.js
|        |__imgs/
|        |  |__logo.png
|        |  |__favicon.ico
```

### Configuration

`docs/themes/fluid/stayle/config.cfg`

```ini
# Path to the HTML layout file
template=docs/themes/desktop/fluid_template.html

# Folder containing CSS/Images/JS to be copied
assets=docs/themes/fluid/stayle/assets

# Target output directory for the website
output=dist/website

# Base path for the website
base_path=https://zheng-bote.github.io/gha_ghpages/

# Enable table of contents
toc=true
```

### Execution

```bash
./build/gh_docs_bot docs/themes/fluid/stayle/config.cfg docs/input_documents
```
