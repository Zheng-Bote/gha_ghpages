# Mermaid Plugin

The **Mermaid Plugin** enables rendering of diagrams and flowcharts using [Mermaid.js](https://mermaid.js.org/).

## How it works

1. It detects Markdown code blocks tagged with `mermaid`.
2. It wraps the content in a `<div class="mermaid">`.
3. It automatically injects the necessary Mermaid.js `<script>` at the end of the page.

## Usage

In your Markdown file:

<pre><code>```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```</code></pre>

## Configuration

Enable in `fluid.cfg`:

```ini
[plugins]
enabled=..., mermaid
```