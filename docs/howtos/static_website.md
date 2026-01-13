# How To Use gh_docs_bot manually

The gh_docs_bot is a custom C++ Static Site Generator (SSG) that allows you to build and generate a static website from your Markdown and HTML documentation. It encapsulates the entire build process, including dependency installation (LLVM 21, etc.), CMake configuration, and execution.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Usage](#usage)
  - [Inputs](#inputs)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

# Usage

`Usage: build/gh_docs_bot <path_to_config> <input_folder>`

**_Example_**

`./gh_docs_bot /home/zb_bamboo/DEV/gh-a_cr-ghp/theme/blue/blue.cfg /home/zb_bamboo/DEV/gh-a_cr-ghp/docs`

## Inputs

| Input            | Description                                      |
| :--------------- | :----------------------------------------------- |
| `path_to_config` | Path to the config file to use for the site.     |
| `path_to_docs`   | Path to the input directory to use for the site. |
