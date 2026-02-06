# Search Plugin

The **Search Plugin** generates a client-side search index.

## How it works

1. It collects titles, URLs, and plain-text content from all pages.
2. It strips HTML tags and normalizes whitespace.
3. After the build, it saves everything into `search.json` in the output root.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., search
```

## Integration

Your theme's JavaScript can fetch the `search.json` file to provide real-time search results without a backend.