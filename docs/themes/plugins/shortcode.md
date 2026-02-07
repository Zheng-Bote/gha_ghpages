# Shortcode Plugin

The **Shortcode Plugin** provides a powerful syntax for embedding dynamic components using `[[ name: args ]]`.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

**Table of Contents**

- [Shortcode Plugin](#shortcode-plugin)
  - [Syntax](#syntax)
  - [Available Shortcodes](#available-shortcodes)
    - [Alert](#alert)
    - [Button](#button)
    - [YouTube](#youtube)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Syntax

```text
[[ name: arg1, key=val, arg2="quoted value" ]]
```

## Available Shortcodes

### Alert

Displays a themed message box.

- **args:** `type` (info, success, warning, error), `text` (or first positional argument).
- **Example:** `[ alert: type=warning, text="This is a warning" ]]` or `[ alert: success, "Action complete" ]]`

[[alert: type=info, text="This is an information"]]
[[alert: type=success, text="This is a success"]]
[[alert: type=warning, text="This is a warning"]]
[[alert: type=error, text="This is an error"]]

### Button

Renders a themed link button.

- **args:** `url`, `text`, `type` (primary, outline, ghost).
- **Example:** `[ button: url="https://google.com", text="Search", type=outline ]]`

[[button: url="https://www.robert.hase-zheng.net", text="my Website", type=primary]]
[[button: url="https://www.hase-zheng.net", text="my other website", type=outline]]
[[button: url="https://www.digidocu.dev", text="another website of mine", type=ghost]]

### YouTube

Embeds a responsive YouTube video.

- **args:** `id` (or first positional argument).
- **Example:** `[ youtube: dQw4w9WgXcQ ]]` or `[ youtube: id=dQw4w9WgXcQ ]]`

[[youtube: gzanWSGXqGc]]

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., shortcode
```

---

> :note: **Note**
> due to the plugins are active, `[]` is used instead of the **usual double-curly braces** (`{ }`) to avoid parsing.
