<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Reading Time Plugin](#reading-time-plugin)
  - [How it works](#how-it-works)
  - [Variables (Template)](#variables-template)
  - [Configuration](#configuration)
  - [Example Usage](#example-usage)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Reading Time Plugin

The **Reading Time Plugin** estimates the reading time based on word count.

## How it works

The plugin counts words in the raw content. If the count exceeds the `threshold`, it calculates the estimated time using the `wpm` (words per minute) setting.

## Variables (Template)

- `[ reading_time ]`: Formatted string (e.g., "5 min read").
- `[ show_reading_time ]`: Boolean, true if content length > threshold.
- `[ word_count ]`: Integer, the total number of words found.

## Configuration

Add to your `fluid.cfg`:

```ini
[plugins]
enabled=..., reading_time

[reading_time]
# Minimum words required to show reading time
threshold=200
# Reading speed (words per minute)
wpm=200
```

## Example Usage

```html
[ if show_reading_time ]
<span class="badge soft">⏱️ { reading_time }</span>
[ endif ]
```

_Note: In the examples above, `[]` is used instead of the usual double-curly braces to avoid parsing issues in some environments._
