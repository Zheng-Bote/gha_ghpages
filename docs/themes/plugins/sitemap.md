<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Sitemap Plugin](#sitemap-plugin)
  - [How it works](#how-it-works)
  - [Configuration](#configuration)
  - [Template Usage](#template-usage)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Sitemap Plugin

The **Sitemap Plugin** generates both XML and HTML sitemaps.

## How it works

The plugin tracks all rendered pages and generates:

1. `sitemap.xml` in the output root (for search engines).
2. `Sitemap/index.html` (a visual page list for users).

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., sitemap

[General]
# Used as the prefix for URLs in sitemap.xml
base_path=https://your-domain.com/
```

## Template Usage

The HTML sitemap is generated automatically as a separate page. You can link to it in your footer:

```html
<a href="[ base_path ]Sitemap/index.html">Sitemap</a>
```
