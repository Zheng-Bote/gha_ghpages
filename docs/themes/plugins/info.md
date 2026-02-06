<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Info Plugin](#info-plugin)
  - [Features](#features)
    - [1. GFM Alerts](#1-gfm-alerts)
    - [2. Emojis](#2-emojis)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Info Plugin

The **Info Plugin** enhances your content by processing GFM (GitHub Flavored Markdown) alerts and standard emoji codes.

## Features

### 1. GFM Alerts

Converts blockquotes with alert markers into styled message boxes.

**Markdown Input:**

```markdown
> [!NOTE]
> Useful information.

> [!TIP]
> Helpful advice.

> [!IMPORTANT]
> Crucial details.

> [!WARNING]
> Urgent info.

> [!CAUTION]
> Risk warning.
```

**Examples**

> [!NOTE]
> Useful information.

> [!TIP]
> Helpful advice.

> [!IMPORTANT]
> Crucial details.

> [!WARNING]
> Urgent info.

> [!CAUTION]
> Risk warning.

**HTML Output:**

```html
<div class="info-msg">
  <p>Useful information.</p>
</div>
<!-- etc -->
```

### 2. Emojis

Replaces common shortcodes with HTML entities or Unicode.

| Code            | Output   |
| :-------------- | :------- |
| `:arrow_right:` | &#10145; |
| `:arrow_left:`  | &#11013; |
| `:warning:`     | &#9888;  |

**Example:**
`:arrow_right: <mark>:warning: Under Construction :warning:</mark> :arrow_left:`

:arrow_right: <mark>:warning: Under Construction :warning:</mark> :arrow_left:

## Configuration

Enable in your `fluid.cfg`:

```ini
[plugins]
enabled=..., info
```
