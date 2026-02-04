# Plugin Developer Guide

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

The SSG is designed to be extended via Plugins using **Shared Libraries** (`.so` on Linux). A plugin can:

1. Provide a **Renderer** for a specific file extension (e.g., `.md`, `.txt`).
2. Hook into the **Build Lifecycle** to modify metadata or content.
3. Hook into the **Markdown Parsing** events (Low-Level).

## 1. The `IPlugin` Interface

All plugins must inherit from `ssg::core::IPlugin` defined in `include/core/interfaces.hpp`.

```cpp
#include "core/interfaces.hpp"

class MyPlugin : public ssg::core::IPlugin {
public:
    std::string name() const override { return "My Plugin"; }
    std::string version() const override { return "1.0"; }
    std::string description() const override { return "Does awesome things"; }

    // Lifecycle Hooks
    void on_init(ssg::model::SiteContext& ctx) override;
    void on_before_render(ssg::model::PageContext& ctx) override;
    void on_after_render(ssg::model::PageContext& ctx) override;

    // Renderer Factory
    std::unique_ptr<ssg::core::IRenderer> create_renderer(const std::string& ext) override;
};
```

## 2. Creating a Simple Plugin (Example)

Let's create a plugin that adds a "Word Count" to every page's metadata.

**Step 1: Implementation (`src/plugins/wordcount_plugin.cpp`)**

```cpp
#include "core/interfaces.hpp"
#include <string>

namespace ssg::plugins {

class WordCountPlugin : public core::IPlugin {
public:
    std::string name() const override { return "Word Counter"; }

    void on_before_render(model::PageContext& ctx) override {
        // Simple word count
        int words = 0;
        bool in_word = false;
        for (char c : ctx.raw_content) {
            if (std::isspace(c)) { in_word = false; }
            else if (!in_word) { in_word = true; words++; }
        }
        ctx.meta_data["word_count"] = words;
    }
};

} // namespace

// --- Export Factory Functions (Required) ---
extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::WordCountPlugin();
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}
```

**Step 2: Compilation (CMake)**
Add your plugin to `CMakeLists.txt` as a SHARED library:

```cmake
add_library(plugin_wordcount SHARED src/plugins/wordcount_plugin.cpp)
target_include_directories(plugin_wordcount PRIVATE include)
```

**Step 3: Configuration**
Update your site config (e.g., `fluid.cfg`) to load the plugin:

```ini
[plugins]
path=plugins
enabled=markdown, html, toc, wordcount
```

_Note: The name in `enabled` corresponds to the suffix of `libplugin_<name>.so`.\_

**Step 4: Use in Template**
In your HTML template:

```html
<p>Words: {{ word_count }}</p>
```

## 3. Advanced: Markdown Hooks & Host Interaction

If your plugin needs to interact with the host (e.g., to trigger parsing events), it should accept `ssg::core::IPluginHost*` in the factory and constructor.

```cpp
class MyComplexPlugin : public core::IPlugin {
    core::IPluginHost& host;
public:
    MyComplexPlugin(core::IPluginHost& h) : host(h) {}
    // ...
};

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new MyComplexPlugin(*host);
}
```

## 4. Multi-Parser Support

To support a new format (e.g., RestructuredText `.rst`):

1. Create a class implementing `ssg::core::IRenderer`.
2. Implement `render(PageContext& ctx)`.
3. In your plugin's `create_renderer`:

```cpp
    std::unique_ptr<IRenderer> create_renderer(const std::string& ext) override {
        if (ext == ".rst") return std::make_unique<RstRenderer>();
        return nullptr;
    }
```
