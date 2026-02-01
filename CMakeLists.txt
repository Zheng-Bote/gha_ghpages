cmake_minimum_required(VERSION 3.23)

# cmake -S . -B build 
# cmake --build build -j$(nproc)  --target gh_docs_bot

project(gh_docs_bot
    VERSION 2.2.1
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
  GIT_TAG v3.11.2 # optional: wähle eine passende Version
)
FetchContent_MakeAvailable(nlohmann_json)

# Optional: Du kannst hier weitere FetchContent-Dependencies hinzufügen

# libcurl via find_package (System-abhängig)
find_package(CURL REQUIRED)

# md4c via FetchContent
FetchContent_Declare(
  md4c
  GIT_REPOSITORY https://github.com/mity/md4c.git
  GIT_TAG release-0.5.2
)
FetchContent_MakeAvailable(md4c)

#target_include_directories(${PROJECT_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# New SSG5 target
add_executable(${PROJECT_NAME}
    src/main.cpp
)


# Include directories (local include for inja.hpp, and md4c headers)
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${md4c_SOURCE_DIR}/src
)

# Link libraries (nlohmann_json and md4c targets)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json 
    md4c-html md4c
)

# Install rule (optional)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
