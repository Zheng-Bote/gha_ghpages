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

*Note: This plugin runs once at the very end of the generation process.*