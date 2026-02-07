# Plugins

Plugins are the way to extend the functionality of the static site generator. they are implemented as shared libraries that are loaded at runtime.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Included Plugins](#included-plugins)
- [Syntax](#syntax)
- [Example](#example)
  - [Output](#output)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Included Plugins

| Name                        | Description                                                 |
| :-------------------------- | :---------------------------------------------------------- |
| **AsciiDoc Renderer**       | Renders `.adoc` files using `asciidoctor` CLI               |
| **Breadcrumb Navigation**   | Generates breadcrumb HTML with folder links                 |
| **Emoji Shortcodes**        | Replaces `shortcode` with HTML entities                     |
| **HTML Renderer**           | Renders HTML files                                          |
| **Info & Alerts**           | Processes GFM-style alerts and specific emojis              |
| **Markdown Renderer**       | Renders Markdown files using `md4c`                         |
| **Mermaid Diagram Support** | Renders `mermaid` code blocks as diagrams                   |
| **Minifier**                | Minifies HTML and CSS output                                |
| **office**                  | Parses ODT, DOCX files to HTML                              |
| **Reading Time Calculator** | Estimates reading time for long content                     |
| **Search Index Generator**  | Generates `search.json` for client-side search              |
| **Shortcode Processor**     | Processes `shortcode` syntax                                |
| **Sitemap Generator**       | Generates `sitemap.xml` and folder-based `index.html` files |
| **TOC & Auto-ID Generator** | Generates Table of Contents and auto-ID headers             |

## Syntax

```bash
./gh_docs_bot "path_to_config" "input_folder"
```

## Example

```bash
./gh_docs_bot "docs/themes/fluid/fluid.cfg" "docs/documents"
```

### Output

```
Loaded Plugin: Markdown Renderer v1.1.0 - Renders Markdown files using md4c
Loaded Plugin: HTML Renderer v0.1.1 - Renders HTML files
Loaded Plugin: TOC & Auto-ID Generator v0.1.3 - Generates Table of Contents and auto-ID headers
Loaded Plugin: AsciiDoc Renderer v0.1.0 - Renders .adoc files using 'asciidoctor' CLI
Loaded Plugin: Search Index Generator v0.1.0 - Generates search.json for client-side search
Loaded Plugin: Sitemap Generator v0.2.1 - Generates sitemap.xml and folder-based index.html files
Loaded Plugin: Mermaid Diagram Support v0.1.0 - Renders mermaid code blocks as diagrams
Loaded Plugin: Breadcrumb Navigation v0.1.2 - Generates breadcrumb HTML with folder links
Loaded Plugin: Reading Time Calculator v0.1.0 - Estimates reading time for long content
Loaded Plugin: Shortcode Processor v0.1.4 - Processes [[ shortcode ]] syntax
Loaded Plugin: Minifier v0.1.1 - Minifies HTML and CSS output
Loaded Plugin: office v0.1.0 - Parses ODT, DOCX files to HTML.
Loaded Plugin: Info & Alerts v0.1.3 - Processes GFM-style alerts and specific emojis.
Initializing...
Scanning structure...
Generating pages...
Generated: dist/index.html
Generated: dist/architecture/overview.html
Generated: dist/architecture/update_v2.html
Generated: dist/development/Plugin_dev_guide.html
Generated: dist/development/build.html
Generated: dist/howtos/Github_Action.html
Generated: dist/howtos/SSG_locally.html
Generated: dist/howtos/User_Guide_v2.html
Generated: dist/screenshots/themes.html
Generated: dist/themes/Fluid_UI.html
Generated: dist/themes/Web_Components.html
Generated: dist/themes/plugins/asciidoc.html
Generated: dist/themes/plugins/breadcrumb.html
Generated: dist/themes/plugins/html.html
Generated: dist/themes/plugins/info.html
Generated: dist/themes/plugins/markdown.html
Generated: dist/themes/plugins/mermaid.html
Generated: dist/themes/plugins/minify.html
Generated: dist/themes/plugins/office.html
Generated: dist/themes/plugins/reading_time.html
Generated: dist/themes/plugins/search.html
Generated: dist/themes/plugins/shortcode.html
Generated: dist/themes/plugins/sitemap.html
Generated: dist/themes/plugins/test.html
Generated: dist/themes/plugins/toc.html
Generated search.json with 25 entries.
Generated sitemap.xml
Generated auto-index for: architecture
Generated auto-index for: development
Generated auto-index for: howtos
Generated auto-index for: screenshots
Generated auto-index for: themes
Generated auto-index for: themes/plugins
Minifying assets in "dist"...
Minified 35 files.
Done! Output in: dist
```
