# Shortcode Plugin

The **Shortcode Plugin** allows inserting complex HTML components using simple placeholders.

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Syntax

```text
[[ name: param1=value, param2=value ]]
```

## Available Shortcodes

### YouTube

Embeds a responsive video.

`[[ youtube: ID ]]` or `[[ youtube: id=ID ]]`

### Alert / Note

Creates a colored alert box.

`[[ alert: text="Message" ]]`
Optional parameter: `type` (info, warning, error, success).

### Button

Creates a link button.

`[[ button: url=https://example.com, text="Click me" ]]`
Optional parameters: `type` (primary, outline, ghost), `alt`, `title`.

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., shortcode
```
