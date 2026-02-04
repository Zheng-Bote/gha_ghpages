# TOC Plugin

The **Table of Contents (TOC) Plugin** automatically generates a table of contents.

## How it works

The plugin hooks into the Markdown parsing process:

1. It intercepts headings (`H2` to `H4`).
2. It automatically generates a URL-friendly ID (slug) for each heading (e.g., "My Heading" -> `#my-heading`).
3. It creates a hierarchical list (`<ul>`) of all headings.
4. The result is stored in the `{{ toc }}` variable.

## Legacy Support

If your template or content still contains old `doctoc` markers, they will also be replaced:

```html
<!-- START doctoc generated TOC ... -->
<!-- END doctoc generated TOC ... -->
```

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., toc
```
