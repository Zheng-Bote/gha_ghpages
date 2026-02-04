<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Search Plugin](#search-plugin)
  - [How it works](#how-it-works)
  - [Configuration](#configuration)
  - [Template Usage](#template-usage)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Search Plugin

The **Search Plugin** generates a search index for client-side search.

## How it works

The plugin collects data from each page during rendering:

- Title
- URL
- Cleaned text content (HTML tags removed)

At the end of the build, a `search.json` file is created in the root directory. This can be loaded by frontend scripts (like Lunr.js or a simple fetch search) to enable full-text search without a server backend.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., search
```

## Template Usage

In your HTML template:

```html
<button id="searchTrigger" class="btn btn-icon btn-ghost" aria-label="Search">
  <svg
    width="20"
    height="20"
    viewBox="0 0 24 24"
    fill="none"
    stroke="currentColor"
    stroke-width="2"
    stroke-linecap="round"
    stroke-linejoin="round"
  >
    <circle cx="11" cy="11" r="8"></circle>
    <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
  </svg>
</button>
```
