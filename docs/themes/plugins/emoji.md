<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Emoji Shortcodes Plugin](#emoji-shortcodes-plugin)
  - [Mechanism](#mechanism)
  - [GitHub Alert Semantics](#github-alert-semantics)
  - [Standard Sets](#standard-sets)
    - [Usage Example](#usage-example)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Emoji Shortcodes Plugin

The **Emoji Shortcodes Plugin** enables the use of intuitive text shortcuts (like `:smile:` or `:warning:`) within your documentation. These shortcodes are automatically replaced with browser-safe HTML entities during the build process.

This works globally in your **Markdown files** (`.md`) as well as in **HTML templates** (`.html`).

## Mechanism

The plugin scans the rendered HTML code for the pattern `:shortcode:` and replaces known keywords with their corresponding HTML entities (e.g., `&#128512;`).

**Benefits:**

- No need to memorize Unicode Hex codes.
- Browser-safe rendering via HTML entities.
- Full support for GitHub-Style Alerts.

---

## GitHub Alert Semantics

This plugin is optimized to support the semantics of [GitHub Alerts](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax#alerts). Use these specific icons inside blockquotes to create visually distinct callouts.

| Meaning       | Icon | Shortcodes                  | Example Markdown                 |
| :------------ | :--: | :-------------------------- | :------------------------------- |
| **NOTE**      |  ℹ️  | `note`, `info`              | `> info **Note:** ...`           |
| **TIP**       |  💡  | `tip`, `bulb`, `hint`       | `> tip **Tip:** ...`             |
| **IMPORTANT** |  ❗  | `important`, `attention`    | `> important **Important:** ...` |
| **WARNING**   |  ⚠️  | `warning`, `warn`           | `> warning **Warning:** ...`     |
| **CAUTION**   |  🛑  | `caution`, `stop`, `danger` | `> caution **Caution:** ...`     |

## Standard Sets

| Category    |       Icons       | Shortcodes                                            |
| :---------- | :---------------: | :---------------------------------------------------- |
| **General** |       ✔ ❌        | `check`, `cross`, `x`                                 |
| **Faces**   | 😀 😂 😉 😎 🤔 😢 | `smile`, `laugh`, `wink`, `cool`, `thinking`, `sad`   |
| **Hands**   |    👍 👎 👌 👏    | `thumbsup`, `thumbsdown`, `ok_hand`, `clap`           |
| **Arrows**  |      ➡ ⬅ ⬆ ⬇      | `arrow_right`, `arrow_left`, `arrow_up`, `arrow_down` |
| **Objects** |    🚀 🔥 ⭐ ❤     | `rocket`, `fire`, `star`, `heart`                     |

### Usage Example

```markdown
> tip **Pro-Tip**
> Use shortcodes to make your documentation more alive.

> warning **Warning**
> Never manually delete system files.
```

**_Output_**

> :tip: **Pro-Tip**
> Use shortcodes to make your documentation more alive.

> :warning: **Warning**
> Never manually delete system files.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., emoji
```

---

> :note: **Note**
> due to the plugin is active, the shortcodes above are all without **colons** `:` before and after the shortcode-names.
