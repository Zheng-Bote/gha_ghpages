cmake_minimum_required(VERSION 3.23)

# cmake -S . -B build 
# cmake --build build -j$(nproc)  --target gh_docs_bot

project(gh_docs_bot
    VERSION 2.2.2
    DESCRIPTION "Static Site Generator"
    HOMEPAGE_URL "https://github.com/Zheng-Bote/gh-docs-bot"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- Configure (für rz_config.hpp) ---
set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(PROG_LONGNAME "Static Site Generator")
set(PROG_CREATED "2025")
set(PROG_AUTHOR "ZHENG Bote")
set(PROG_ORGANIZATION_NAME "ZHENG Robert")
set(PROG_ORGANIZATION_DOMAIN "net.hase-zheng")
# create rz_config.hpp
add_subdirectory(configure) 

include(FetchContent)

# nlohmann/json via FetchContent
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2 
)
FetchContent_MakeAvailable(nlohmann_json)

# libcurl via find_package (System-abhängig)
find_package(CURL REQUIRED)

# md4c via FetchContent
FetchContent_Declare(
  md4c
  GIT_REPOSITORY https://github.com/mity/md4c.git
  GIT_TAG release-0.5.2
)
FetchContent_MakeAvailable(md4c)

# inifile-cpp via FetchContent
FetchContent_Declare(
  inifile-cpp
  GIT_REPOSITORY https://github.com/Rookfighter/inifile-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(inifile-cpp)

# pugixml via FetchContent
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG v1.14
)
FetchContent_MakeAvailable(pugixml)

# miniz via FetchContent (using a cmake-friendly mirror or wrapper if needed, but miniz is single file usually. 
# We'll use a repo that provides CMake support or just add it manually if it was local. 
# Since we need to fetch it, let's use a reliable CMake fork or just standard miniz and add library manually if needed.
# Actually, let's use kubazip which wraps miniz nicely for C++, or just raw miniz.
# Let's try raw miniz from a repo that has CMakeLists.
FetchContent_Declare(
  miniz
  GIT_REPOSITORY https://github.com/richgel999/miniz.git
  GIT_TAG master
)
FetchContent_MakeAvailable(miniz)
# Note: miniz upstream might not have CMakeLists.txt. 
# If it fails, we will create a simple target for it. 
# Checking typical usage: usually people just drop miniz.c/.h in. 
# Let's assume for this environment we might need a backup. 
# Actually, let's use 'zip' library which is C++ wrapper around miniz: https://github.com/kuba--/zip

# zip (C++ wrapper for miniz)
FetchContent_Declare(
  libzip
  GIT_REPOSITORY https://github.com/kuba--/zip.git
  GIT_TAG master
)
FetchContent_MakeAvailable(libzip)
# Suppress warnings for the 3rd party zip library
if(TARGET zip)
    target_compile_options(zip PRIVATE -w)
endif()

# --- Main Executable ---
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/core/site_builder.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${md4c_SOURCE_DIR}/src
    ${inifile-cpp_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json 
    dl
)

# --- Plugins ---

# Office Plugin (ODT/DOCX)
add_library(plugin_office SHARED src/plugins/office_plugin.cpp)
target_include_directories(plugin_office PRIVATE include ${md4c_SOURCE_DIR}/src)
target_link_libraries(plugin_office PRIVATE pugixml::pugixml zip)
set_target_properties(plugin_office PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_office"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Markdown Plugin
add_library(plugin_markdown SHARED src/plugins/markdown_plugin.cpp)
target_include_directories(plugin_markdown PRIVATE include ${md4c_SOURCE_DIR}/src)
target_link_libraries(plugin_markdown PRIVATE md4c-html md4c)
set_target_properties(plugin_markdown PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_markdown"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# HTML Plugin
add_library(plugin_html SHARED src/plugins/html_plugin.cpp)
target_include_directories(plugin_html PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_html PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_html"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# TOC Plugin
add_library(plugin_toc SHARED src/plugins/toc_plugin.cpp)
target_include_directories(plugin_toc PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_toc PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_toc"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# AsciiDoc Plugin
add_library(plugin_asciidoc SHARED src/plugins/asciidoc_plugin.cpp)
target_include_directories(plugin_asciidoc PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_asciidoc PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_asciidoc"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Search Plugin
add_library(plugin_search SHARED src/plugins/search_plugin.cpp)
target_include_directories(plugin_search PRIVATE include ${md4c_SOURCE_DIR}/src)
target_link_libraries(plugin_search PRIVATE nlohmann_json::nlohmann_json)
set_target_properties(plugin_search PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_search"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Sitemap Plugin
add_library(plugin_sitemap SHARED src/plugins/sitemap_plugin.cpp)
target_include_directories(plugin_sitemap PRIVATE include ${md4c_SOURCE_DIR}/src)
target_link_libraries(plugin_sitemap PRIVATE nlohmann_json::nlohmann_json)
set_target_properties(plugin_sitemap PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_sitemap"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Mermaid Plugin
add_library(plugin_mermaid SHARED src/plugins/mermaid_plugin.cpp)
target_include_directories(plugin_mermaid PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_mermaid PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_mermaid"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Breadcrumb Plugin
add_library(plugin_breadcrumb SHARED src/plugins/breadcrumb_plugin.cpp)
target_include_directories(plugin_breadcrumb PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_breadcrumb PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_breadcrumb"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Minify Plugin
add_library(plugin_minify SHARED src/plugins/minify_plugin.cpp)
target_include_directories(plugin_minify PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_minify PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_minify"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Reading Time Plugin
add_library(plugin_reading_time SHARED src/plugins/reading_time_plugin.cpp)
target_include_directories(plugin_reading_time PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_reading_time PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_reading_time"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Shortcode Plugin
add_library(plugin_shortcode SHARED src/plugins/shortcode_plugin.cpp)
target_include_directories(plugin_shortcode PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_shortcode PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_shortcode"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Info Plugin
add_library(plugin_info SHARED src/plugins/info_plugin.cpp)
target_include_directories(plugin_info PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_info PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_info"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Emoji Plugin
add_library(plugin_emoji SHARED src/plugins/emoji_plugin.cpp)
target_include_directories(plugin_emoji PRIVATE include ${md4c_SOURCE_DIR}/src)
set_target_properties(plugin_emoji PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "plugin_emoji"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Install rule
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(TARGETS plugin_markdown plugin_html plugin_toc plugin_asciidoc plugin_search plugin_sitemap plugin_mermaid plugin_breadcrumb plugin_minify plugin_reading_time plugin_shortcode plugin_office plugin_info plugin_emoji LIBRARY DESTINATION bin/plugins)

