# TOC Plugin

The **Table of Contents (TOC) Plugin** automatically generates a hierarchical list of headings.

## How it works

The plugin scans the rendered content for `H2`, `H3`, and `H4` tags, generates unique IDs for them, and builds a nested list.

## Variables (Template)

- `[ toc ]`: The generated HTML list for the table of contents.

## Legacy Support

Replaces old `doctoc` markers if present:

```html
<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Example Usage](#example-usage)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->
```

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., toc
```

## Example Usage

```html
<aside class="sidebar">
  <h3>Contents</h3>
  [ toc ]
</aside>
```

_Note: In the examples above, `[]` is used instead of the usual double-curly braces to avoid parsing issues in some environments._
