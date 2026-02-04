# AsciiDoc Plugin

The **AsciiDoc Plugin** allows rendering of `.adoc` and `.asciidoc` files.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [How it works](#how-it-works)
- [Prerequisites](#prerequisites)
- [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## How it works

The plugin uses the installed `asciidoctor` CLI to convert content to HTML5. It supports standard AsciiDoc features like:

- Formatting (bold, italic, etc.)
- Lists
- Tables
- Admonitions (Note/Warning boxes)
- Code blocks

The document title (`= Title`) is automatically extracted and used for page metadata.

## Prerequisites

The `asciidoctor` tool must be installed on your system and available in the `PATH`.

```bash
# Ubuntu/Debian
sudo apt-get install asciidoctor

# Ruby Gem
gem install asciidoctor
```

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., asciidoc
```
