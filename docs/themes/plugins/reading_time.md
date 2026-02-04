# Reading Time Plugin

The **Reading Time Plugin** estimates the reading time for a page's content.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [How it works](#how-it-works)
- [Variables](#variables)
- [Configuration](#configuration)
- [Template Usage](#template-usage)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## How it works

The plugin counts words in the raw content of the page. If the word count exceeds a configurable threshold, an estimated reading time is calculated and made available.

## Variables

- `{ reading_time }}`: Text string like "5 min read".
- `{ show_reading_time }}`: `true` if the text is long enough, otherwise `false`.
- `{ word_count }}`: The raw word count.

## Configuration

In `fluid.cfg`:

```ini
[plugins]
enabled=..., reading_time

[reading_time]
# Minimum number of words to display reading time
threshold=200
# Words per minute (reading speed)
wpm=200
```

## Template Usage

In your HTML template:

```html
<main class="site-main">
  <div class="flex justify-between items-center mb-3">
    { breadcrumbs }} % if show_reading_time }
    <span class="badge soft">⏱️ { reading_time }}</span>
    % endif %}
  </div>
  { content }}
</main>
```
