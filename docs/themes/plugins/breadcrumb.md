<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Breadcrumb Plugin](#breadcrumb-plugin)
  - [How it works](#how-it-works)
  - [Variables (Template)](#variables-template)
  - [Configuration](#configuration)
  - [Example Usage](#example-usage)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Breadcrumb Plugin

The **Breadcrumb Plugin** generates a navigation trail based on the site's directory structure.

## How it works

The plugin analyzes the relative path of the current page and creates a sequence of links back to the root.

## Variables (Template)

- `[ breadcrumbs ]`: The generated HTML for the breadcrumb trail (e.g., `Home / Docs / Components`).

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., breadcrumb
```

## Example Usage

```html
<div class="header-breadcrumb">
  [ breadcrumbs ]
</div>
```