# Breadcrumb Plugin

The **Breadcrumb Plugin** automatically generates a breadcrumb navigation path.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## How it works

Based on the directory structure of the source file, an HTML fragment is created that represents the path from the home page to the current file.

- Folders are displayed as intermediate steps.
- Since not every folder necessarily has an `index.html`, intermediate folders link to `#`.
- The path is injected into the `{ breadcrumbs }}` template variable.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., breadcrumb
```

## Template Usage

In your HTML template:

```html
<main>{ breadcrumbs }} { content }}</main>
```
