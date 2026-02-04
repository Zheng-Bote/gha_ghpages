# Markdown Plugin

The **Markdown Plugin** is the default renderer for `.md` and `.markdown` files.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [How it works](#how-it-works)
- [Features](#features)
- [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## How it works

It uses the **MD4C** (Markdown for C) library to convert GitHub Flavored Markdown (GFM) to HTML.

## Features

- **GitHub Flavored Markdown:** Supports tables, strikethrough, autolinks, task lists.
- **Performance:** MD4C is extremely fast and memory-efficient.
- **Hook Support:** The plugin forwards parsing events to the `PluginManager`, allowing other plugins (like `TocPlugin` or `MermaidPlugin`) to intervene in the rendering process.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=markdown, ...
```
