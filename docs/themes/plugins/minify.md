# Minify Plugin

The **Minify Plugin** optimizes the generated website by reducing file sizes.

## How it works

The plugin runs after the build process (`on_post_build`) and processes files in the output directory.

- **HTML:** Removes comments (`<!-- ... -->`) and unnecessary whitespace between tags.
- **CSS:** Removes comments (`/* ... */`) and whitespace.
- **JS:** Currently not minified to avoid code breaking (regex minification is risky for JS).

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., minify
```
