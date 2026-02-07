<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [HTML Plugin](#html-plugin)
  - [How it works](#how-it-works)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# HTML Plugin

The **HTML Plugin** allows you to use standard `.html` and `.htm` files as source content.

## How it works

The plugin simply reads the HTML file and provides it as the `[ content ]` for the template. This allows you to mix hand-crafted HTML pages with generated content while keeping the same site theme and navigation.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., html
```
