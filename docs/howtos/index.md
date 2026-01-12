# How To Use GH Docs Bot Action

This action allows you to build and generate a static website from your Markdown documentation using the custom C++ Static Site Generator (SSG). It encapsulates the entire build process, including dependency installation (LLVM 21, etc.), CMake configuration, and execution.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Inputs](#inputs)
- [Example Workflow](#example-workflow)
- [Prerequisites](#prerequisites)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Inputs

| Input      | Description                                              | Default               | Required |
| :--------- | :------------------------------------------------------- | :-------------------- | :------- |
| `template` | Path to the HTML template file to use for the site.      | `theme/template.html` | No       |
| `output`   | Directory where the generated HTML files will be placed. | `public`              | No       |
| `docs`     | Directory containing your source Markdown files.         | `docs`                | No       |

## Example Workflow

Here is a complete example of how to use this action in your own `.github/workflows/deploy-docs.yml` file. This example builds the site and then deploys it to GitHub Pages.

```yaml
name: Deploy Documentation

on:
  push:
    branches:
      - main
  workflow_dispatch:

permissions:
  contents: write

jobs:
  build-and-deploy:
    runs-on: ubuntu-24.04

    steps:
      - name: Checkout repository
        uses: actions/checkout@v4

      - name: Build and Generate Site
        uses: Zheng-Bote/gha_ghpages@main # or @v1.0.0
        with:
          template: "theme/template.html"
          output: "public"
          docs: "docs"

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
