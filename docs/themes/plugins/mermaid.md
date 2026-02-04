# Mermaid Plugin

The **Mermaid Plugin** enables rendering of diagrams directly in the browser.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## How it works

The plugin searches Markdown files for code blocks with the language `mermaid`:

    ```mermaid
    graph TD;
        A-->B;
        A-->C;
    ```

It converts these blocks into `<div class="mermaid">` containers and automatically injects the Mermaid.js script from a CDN if diagrams are present on the page.

## Features

- **HTML Escaping:** Special characters in diagram text (like `<` or `>`) are correctly escaped so the browser doesn't interpret them as HTML tags.
- **Auto-Load:** The script is only loaded when needed.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., mermaid
```
