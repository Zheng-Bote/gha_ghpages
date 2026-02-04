# Shortcode Plugin

The **Shortcode Plugin** allows inserting complex HTML components using simple placeholders.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

**Table of Contents**

- [Shortcode Plugin](#shortcode-plugin)
  - [Syntax](#syntax)
  - [Available Shortcodes](#available-shortcodes)
    - [YouTube](#youtube)
      - [Example Youtube](#example-youtube)
    - [Alert / Note](#alert--note)
      - [Example Alert](#example-alert)
    - [Button](#button)
      - [Example Button](#example-button)
  - [Configuration](#configuration)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Syntax

```text
[ name: param1=value, param2=value ]]
```

## Available Shortcodes

### YouTube

Embeds a responsive video.

```text
[ youtube: ID ]]
```

or

```text
[ youtube: id=ID ]]
```

#### Example Youtube

[[youtube: id=xp0sFFEwvic]]

### Alert / Note

Creates a colored alert box.

```text
[ alert: type=info, text="This is an Info message" ]]
```

Optional parameter: `type` (info, warning, error, success).

#### Example Alert

[[alert: type=info, text="This is an info"]]

[[alert: type=warning, text="This is a warning"]]

[[alert: type=error, text="This is an error"]]

[[alert: type=success, text="This is a success"]]

### Button

Creates a link button.

```text
[ button: url=https://example.com, text="Click me" ]]
```

#### Example Button

[[button: url=https://www.robert.hase-zheng.net/, alt="my Website", text="My Website"]]

[[button: url=https://www.robert.hase-zheng.net/, alt="my Website", text="My Website", type="secondary"]]

[[button: url=https://www.robert.hase-zheng.net/, alt="my Website", text="My Website", type="ghost"]]

## Configuration

Enable in `fluid.cfg`:
[plugins]
enabled=..., shortcode

```

```
