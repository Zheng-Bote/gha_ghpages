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