<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Minify Plugin](#minify-plugin)
  - [How it works](#how-it-works)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Minify Plugin

The **Minify Plugin** reduces the file size of the generated site by removing unnecessary characters.

## How it works

After the build is complete, the plugin scans the output directory:

1. **HTML:** Removes comments and collapses whitespace between tags.
2. **CSS:** Removes comments, collapses whitespace, and removes spaces around delimiters (`{`, `:`, `;`, etc.).

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., minify
```

_Note: This plugin runs once at the very end of the generation process._
