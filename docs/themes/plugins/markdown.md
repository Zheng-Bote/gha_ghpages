# Markdown Plugin

The **Markdown Plugin** is the core renderer for `.md` and `.markdown` files.

## How it works

It uses the high-performance **md4c** library to parse GitHub Flavored Markdown (GFM).

## Features

- **GFM Support:** Tables, Task lists, Autolinks, and Strikethrough.
- **Plugin Hooks:** Other plugins (like Mermaid or TOC) can hook into the parsing process to intercept specific elements.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=markdown, ...
```