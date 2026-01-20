# How To Use GH Docs Bot Action

This action allows you to build and generate a static website from your Markdown documentation using the custom C++ Static Site Generator (SSG). It encapsulates the entire build process, including dependency installation (LLVM 21, etc.), CMake configuration, and execution.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

**Table of Contents**

- [How To Use GH Docs Bot Action](#how-to-use-gh-docs-bot-action)
  - [Inputs](#inputs)
  - [Example Workflow](#example-workflow)
  - [Prerequisites](#prerequisites)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Inputs

| Input       | Description                                              | Default                                     | Required |
| :---------- | :------------------------------------------------------- | :------------------------------------------ | :------- |
| `template`  | Path to the HTML template file to use for the site.      | `theme/desktop/template.html`               | No       |
| `assets`    | Path to the assets directory to use for the site.        | `theme/blue/assets`                         | No       |
| `output`    | Directory where the generated HTML files will be placed. | `public`                                    | No       |
| `docs`      | Directory containing your source Markdown / HTM files.   | `docs`                                      | No       |
| `base_path` | Base path for the generated site                         | `https://zheng-bote.github.io/gha_ghpages/` | No       |

## Example Workflow

Here is a complete example of how to use this action in your own `.github/workflows/deploy-docs.yml` file. This example builds the site and then deploys it to GitHub Pages.

```yaml
name: Deploy Documentation

name: Manual gh_docs_bot Build

# Author: Robert Zheng
# created: 2026-01-12
# updated: 2026-01-12
# version: 1.0.0
# description: build and generate static website from markdown files to branch gh-pages

run-name: Manual deploy documentation by ${{ github.actor }}

on:
  workflow_dispatch:

permissions:
  contents: write

jobs:
  build-and-generate:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
        with:
          fetch-depth: 0
          ref: "main"
      - name: Build and Generate Site
        uses: Zheng-Bote/gha_ghpages@main
        with:
          template: 'theme/desktop/template.html'
          assets: 'theme/blue/assets'
          output: 'public'
          docs: 'docs'
      - name: Upload Artifact
        uses: actions/upload-artifact@v4
        with:
          name: static-site
          path: public/

      - name: Deploy to gh-pages
        uses: JamesIves/github-pages-deploy-action@v4
        with:
          branch: gh-pages
          folder: public

```

## Prerequisites

Ensure your repository has the following structure:

- `docs/`: A directory containing your Markdown files.
- `theme/template.html`: An HTML template file (consistent with `inja` templates).
- `CMakeLists.txt` & `src/`: If you are building the generator from source code in the same repo (as this action compiles it).
