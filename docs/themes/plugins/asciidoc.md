<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [AsciiDoc Plugin](#asciidoc-plugin)
  - [How it works](#how-it-works)
  - [Requirements](#requirements)
  - [Features](#features)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# AsciiDoc Plugin

The **AsciiDoc Plugin** allows you to use `.adoc` and `.asciidoc` files as source content.

## How it works

The plugin uses the `asciidoctor` command-line tool to render files into HTML fragments.

## Requirements

You must have `asciidoctor` installed on your system:

```bash
sudo apt-get install asciidoctor
```

## Features

- **Title Extraction:** Automatically extracts the document title from the AsciiDoc header and populates the `[ title ]` variable.
- **Full Support:** Supports all standard AsciiDoc features including includes, attributes, and cross-references.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., asciidoc
```
