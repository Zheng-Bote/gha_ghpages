# Office Plugin

The **Office Plugin** allows you to use Microsoft Word (`.docx`) and OpenDocument Text (`.odt`) files as source content for your static site.

## How it works

The plugin intercepts files with `.odt` or `.docx` extensions:

1. It unzips the document (Office files are essentially ZIP archives).
2. It extracts the core XML content (`content.xml` for ODT, `word/document.xml` for DOCX).
3. It parses the XML using `pugixml` and translates it into standard HTML.
4. The generated HTML is then processed like any other page content.

## Supported Features

- **Headers:** Translates ODT/DOCX headings to `<h1>` through `<h6>`.
- **Paragraphs:** Standard text blocks.
- **Lists:** Ordered and unordered lists (ODT).
- **Tables:** Basic table structures (DOCX).
- **Links:** Hyperlinks are preserved.

## Configuration

Enable in your theme's `.cfg` file (e.g., `fluid.cfg`):

```ini
[plugins]
enabled=..., office
```

## Template Usage

Since the plugin converts the document directly to the main content, you use the standard `[ content ]` variable in your template:

```html
<article class="site-main">
  <h1>[ title ]</h1>
  [ content ]
</article>
```

_Note: In the examples above, `[]` is used instead of the usual double-curly braces to avoid parsing issues in some environments._
