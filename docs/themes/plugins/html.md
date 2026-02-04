# HTML Plugin

The **HTML Plugin** is a simple pass-through renderer for `.html` and `.htm` files.

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

Files with the `.html` extension are not converted but embedded directly into the page template. This is useful for pages requiring specific HTML markup that is difficult to represent in Markdown.

## Features

- **Header Extraction:** The plugin scans HTML content for `<h2>` to `<h4>` tags with `id` attributes and adds them to the Table of Contents (TOC).

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., html
```
