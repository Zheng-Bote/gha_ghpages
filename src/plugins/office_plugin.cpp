/**
 * SPDX-FileComment: Office Plugin Implementation
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file office_plugin.cpp
 * @brief Implementation of the Office Plugin using pugixml and zip.
 * @version 0.1.0
 * @date 2026-02-06
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#include "plugins/office_plugin.hpp"
#include <pugixml.hpp>
#include <zip.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <format>
#include <cstdlib>
#include <cstring>

namespace ssg::plugins {

class OfficeRenderer : public core::IRenderer {
    core::IPluginHost& host;
    std::string extension;

public:
    OfficeRenderer(core::IPluginHost& h, std::string ext) : host(h), extension(std::move(ext)) {}

    std::expected<std::string, std::string> render(model::PageContext& ctx) override {
        try {
            if (!std::filesystem::exists(ctx.source_path)) {
                return std::unexpected("Source file not found: " + ctx.source_path.string());
            }

            // Unzip content.xml
            std::string xml_content;
            if (extension == ".odt") {
                xml_content = extract_zip_file(ctx.source_path.string(), "content.xml");
            } else if (extension == ".docx") {
                xml_content = extract_zip_file(ctx.source_path.string(), "word/document.xml");
            } else {
                return std::unexpected("Unsupported format for Office plugin");
            }

            if (xml_content.empty()) {
                return std::unexpected("Could not extract content XML from document");
            }

            // Parse XML
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_string(xml_content.c_str());

            if (!result) {
                return std::unexpected(std::string("XML Parsing failed: ") + result.description());
            }

            std::stringstream html;
            
            if (extension == ".odt") {
                convert_odt(doc, html);
            } else if (extension == ".docx") {
                convert_docx(doc, html);
            }

            return html.str();

        } catch (const std::exception& e) {
            return std::unexpected(std::string("Exception during Office conversion: ") + e.what());
        }
    }

private:
    std::string extract_zip_file(const std::string& zip_path, const std::string& target_file) {
        struct zip_t *zip = zip_open(zip_path.c_str(), 0, 'r');
        if (!zip) throw std::runtime_error("Failed to open zip file");
        
        if (zip_entry_open(zip, target_file.c_str()) < 0) {
            zip_close(zip);
            return ""; // Not found
        }
        
        void *buf = NULL;
        size_t size = 0;
        if (zip_entry_read(zip, &buf, &size) < 0) {
            zip_entry_close(zip);
            zip_close(zip);
            throw std::runtime_error("Failed to read zip entry");
        }
        
        std::string content(static_cast<char*>(buf), size);
        free(buf); // kuba-zip allocates buffer
        
        zip_entry_close(zip);
        zip_close(zip);
        return content;
    }

    void convert_odt(pugi::xml_document& doc, std::stringstream& out) {
        // Root: office:document-content -> office:body -> office:text
        auto root = doc.child("office:document-content").child("office:body").child("office:text");
        if (!root) return;

        process_odt_nodes(root, out);
    }

    void process_odt_nodes(pugi::xml_node& node, std::stringstream& out) {
        for (pugi::xml_node child : node.children()) {
            std::string name = child.name();
            
            if (name == "text:h") {
                // Header
                std::string level = child.attribute("text:outline-level").value();
                if (level.empty()) level = "1";
                out << "<h" << level << ">";
                process_odt_text(child, out);
                out << "</h" << level << ">\n";
            } else if (name == "text:p") {
                // Paragraph
                out << "<p>";
                process_odt_text(child, out);
                out << "</p>\n";
            } else if (name == "text:list") {
                // List
                out << "<ul>\n";
                process_odt_nodes(child, out);
                out << "</ul>\n";
            } else if (name == "text:list-item") {
                // List Item
                out << "<li>";
                process_odt_nodes(child, out);
                out << "</li>\n";
            } else {
                // Recurse for other containers
                process_odt_nodes(child, out);
            }
        }
    }

    void process_odt_text(pugi::xml_node& node, std::stringstream& out) {
        for (pugi::xml_node child : node.children()) {
            std::string name = child.name();
            if (name.empty()) { // Text node
                out << child.value();
            } else if (name == "text:span") {
                out << "<span>";
                process_odt_text(child, out);
                out << "</span>";
            } else if (name == "text:a") {
                std::string href = child.attribute("xlink:href").value();
                out << "<a href=\"" << href << "\">";
                process_odt_text(child, out);
                out << "</a>";
            } else if (name == "text:s") { // Space
                out << " ";
            } else if (name == "text:tab") {
                out << "\t";
            } else {
                process_odt_text(child, out);
            }
        }
    }

    void convert_docx(pugi::xml_document& doc, std::stringstream& out) {
        // Root: w:document -> w:body
        auto body = doc.child("w:document").child("w:body");
        if (!body) return;
        
        process_docx_nodes(body, out);
    }

    void process_docx_nodes(pugi::xml_node& node, std::stringstream& out) {
        for (pugi::xml_node child : node.children()) {
            std::string name = child.name();
            
            if (name == "w:p") {
                // Paragraph (or Header if it has style)
                // Check style
                std::string style = child.child("w:pPr").child("w:pStyle").attribute("w:val").value();
                
                // Very basic heuristic
                if (style.starts_with("Heading")) {
                     char level = style.back(); // '1' from 'Heading1'
                     if (isdigit(level)) {
                        out << "<h" << level << ">";
                        process_docx_text(child, out);
                        out << "</h" << level << ">\n";
                     } else {
                        out << "<h1>";
                        process_docx_text(child, out);
                        out << "</h1>\n";
                     }
                } else {
                    out << "<p>";
                    process_docx_text(child, out);
                    out << "</p>\n";
                }
            } else if (name == "w:tbl") {
                 out << "<table>\n";
                 // Rows w:tr, Cells w:tc
                 for(auto row : child.children("w:tr")) {
                     out << "<tr>";
                     for(auto cell : row.children("w:tc")) {
                         out << "<td>";
                         process_docx_nodes(cell, out); // recursive for paragraphs in cell
                         out << "</td>";
                     }
                     out << "</tr>\n";
                 }
                 out << "</table>\n";
            }
        }
    }

    void process_docx_text(pugi::xml_node& node, std::stringstream& out) {
        // Runs w:r
        for (auto run : node.children("w:r")) {
             // Text w:t
             for (auto text : run.children("w:t")) {
                 out << text.text().get();
             }
        }
        // Hyperlinks w:hyperlink
        for (auto link : node.children("w:hyperlink")) {
             process_docx_nodes(link, out);
        }
    }
};

std::unique_ptr<core::IRenderer> OfficePlugin::create_renderer(const std::string& extension) {
    if (extension == ".odt" || extension == ".docx") {
        return std::make_unique<OfficeRenderer>(host, extension);
    }
    return nullptr;
}

} // namespace ssg::plugins

extern "C" ssg::core::IPlugin* create_plugin(ssg::core::IPluginHost* host) {
    return new ssg::plugins::OfficePlugin(*host);
}

extern "C" void destroy_plugin(ssg::core::IPlugin* plugin) {
    delete plugin;
}