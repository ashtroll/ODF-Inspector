#include "ODFInspector.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

ODFInspector::ODFInspector(const std::string& odfPath)
    : odfPath_(odfPath)
    , zipReader_(std::make_unique<ZipReader>(odfPath))
    , isLoaded_(false) {
}

ODFInspector::~ODFInspector() = default;

bool ODFInspector::load() {
    if (!zipReader_->open()) {
        lastError_ = "Failed to open ODF file: " + zipReader_->getLastError();
        return false;
    }

    if (!validateODF()) {
        lastError_ = "Invalid ODF file";
        return false;
    }

    if (!extractCoreFiles()) {
        lastError_ = "Failed to extract core ODF files";
        return false;
    }

    isLoaded_ = true;
    return true;
}

bool ODFInspector::validateODF() {
    // Check for mimetype file
    if (!zipReader_->fileExists("mimetype")) {
        return false;
    }

    mimeType_ = zipReader_->extractFile("mimetype");
    
    // Remove any trailing whitespace
    mimeType_.erase(std::remove_if(mimeType_.begin(), mimeType_.end(), ::isspace), mimeType_.end());

    // Check if it's a valid ODF MIME type
    return mimeType_.find("application/vnd.oasis.opendocument") == 0;
}

bool ODFInspector::extractCoreFiles() {
    // Extract key XML files
    if (zipReader_->fileExists("content.xml")) {
        contentXml_ = zipReader_->extractFile("content.xml");
    }

    if (zipReader_->fileExists("meta.xml")) {
        metaXml_ = zipReader_->extractFile("meta.xml");
    }

    if (zipReader_->fileExists("styles.xml")) {
        stylesXml_ = zipReader_->extractFile("styles.xml");
    }

    if (zipReader_->fileExists("META-INF/manifest.xml")) {
        manifestXml_ = zipReader_->extractFile("META-INF/manifest.xml");
    }

    return true;
}

void ODFInspector::displaySummary() const {
    if (!isLoaded_) {
        std::cout << "ODF file not loaded\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "ODF INSPECTOR SUMMARY\n";
    std::cout << "========================================\n\n";
    std::cout << "File: " << odfPath_ << "\n";
    std::cout << "Type: " << getDocumentType() << "\n";
    std::cout << "MIME: " << mimeType_ << "\n";
    std::cout << "Valid ODF: " << (isValidODF() ? "Yes" : "No") << "\n\n";

    // File count
    auto files = zipReader_->listFiles();
    std::cout << "Total files in archive: " << files.size() << "\n";
    
    // Core files present
    std::cout << "\nCore Files:\n";
    std::cout << "  - content.xml: " << (!contentXml_.empty() ? "Present" : "Missing") << "\n";
    std::cout << "  - meta.xml: " << (!metaXml_.empty() ? "Present" : "Missing") << "\n";
    std::cout << "  - styles.xml: " << (!stylesXml_.empty() ? "Present" : "Missing") << "\n";
    std::cout << "  - manifest.xml: " << (!manifestXml_.empty() ? "Present" : "Missing") << "\n";
    
    std::cout << "========================================\n\n";
}

void ODFInspector::displayStructure() const {
    if (!isLoaded_) {
        std::cout << "ODF file not loaded\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "FILE STRUCTURE\n";
    std::cout << "========================================\n\n";

    auto files = zipReader_->listFiles();
    for (const auto& file : files) {
        size_t size = zipReader_->getFileSize(file);
        std::cout << "  " << std::setw(40) << std::left << file 
                  << std::setw(10) << std::right << size << " bytes\n";
    }

    std::cout << "\n========================================\n\n";
}

void ODFInspector::displayMetadata() const {
    if (!isLoaded_ || metaXml_.empty()) {
        std::cout << "Metadata not available\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "METADATA (meta.xml)\n";
    std::cout << "========================================\n\n";

    auto metadata = parseMetadata();
    
    if (metadata.empty()) {
        std::cout << "No metadata extracted\n";
    } else {
        for (const auto& [key, value] : metadata) {
            std::cout << "  " << key << ": " << value << "\n";
        }
    }

    std::cout << "\n========================================\n\n";
}

void ODFInspector::displayContent() const {
    if (!isLoaded_ || contentXml_.empty()) {
        std::cout << "Content not available\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "CONTENT PREVIEW (content.xml)\n";
    std::cout << "========================================\n\n";

    // Display first 2000 characters of formatted XML
    std::string formatted = formatXML(contentXml_);
    std::string preview = formatted.substr(0, std::min(size_t(2000), formatted.size()));
    
    std::cout << preview;
    
    if (formatted.size() > 2000) {
        std::cout << "\n\n... (truncated, " << (formatted.size() - 2000) 
                  << " more characters)\n";
    }

    std::cout << "\n========================================\n\n";
}

void ODFInspector::displayStyles() const {
    if (!isLoaded_ || stylesXml_.empty()) {
        std::cout << "Styles not available\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "STYLES (styles.xml)\n";
    std::cout << "========================================\n\n";

    // Display first 1500 characters of formatted XML
    std::string formatted = formatXML(stylesXml_);
    std::string preview = formatted.substr(0, std::min(size_t(1500), formatted.size()));
    
    std::cout << preview;
    
    if (formatted.size() > 1500) {
        std::cout << "\n\n... (truncated)\n";
    }

    std::cout << "\n========================================\n\n";
}

void ODFInspector::displayManifest() const {
    if (!isLoaded_ || manifestXml_.empty()) {
        std::cout << "Manifest not available\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "MANIFEST (META-INF/manifest.xml)\n";
    std::cout << "========================================\n\n";

    std::cout << formatXML(manifestXml_) << "\n";

    std::cout << "========================================\n\n";
}

void ODFInspector::displayFile(const std::string& filename) const {
    if (!isLoaded_) {
        std::cout << "ODF file not loaded\n";
        return;
    }

    if (!zipReader_->fileExists(filename)) {
        std::cout << "File '" << filename << "' not found in archive\n";
        return;
    }

    std::string content = zipReader_->extractFile(filename);
    
    std::cout << "\n========================================\n";
    std::cout << "FILE: " << filename << "\n";
    std::cout << "SIZE: " << content.size() << " bytes\n";
    std::cout << "========================================\n\n";

    // If it's XML, format it
    if (filename.find(".xml") != std::string::npos) {
        std::cout << formatXML(content) << "\n";
    } else {
        std::cout << content << "\n";
    }

    std::cout << "========================================\n\n";
}

void ODFInspector::listImages() const {
    if (!isLoaded_) {
        std::cout << "ODF file not loaded\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "EMBEDDED IMAGES\n";
    std::cout << "========================================\n\n";

    auto files = zipReader_->listFiles();
    bool foundImages = false;

    for (const auto& file : files) {
        if (file.find("Pictures/") == 0 || file.find("images/") == 0) {
            size_t size = zipReader_->getFileSize(file);
            std::cout << "  " << file << " (" << size << " bytes)\n";
            foundImages = true;
        }
    }

    if (!foundImages) {
        std::cout << "  No embedded images found\n";
    }

    std::cout << "\n========================================\n\n";
}

std::string ODFInspector::getMimeType() const {
    return mimeType_;
}

std::string ODFInspector::getDocumentType() const {
    return getDocTypeFromMime(mimeType_);
}

bool ODFInspector::isValidODF() const {
    return isLoaded_ && !mimeType_.empty();
}

std::string ODFInspector::getLastError() const {
    return lastError_;
}

std::string ODFInspector::formatXML(const std::string& xml) const {
    // Basic XML formatting (add line breaks after tags)
    std::string formatted;
    formatted.reserve(xml.size() * 1.2);
    
    int indent = 0;
    bool inTag = false;
    bool afterClosingBracket = false;
    
    for (size_t i = 0; i < xml.size(); ++i) {
        char c = xml[i];
        
        if (c == '<') {
            if (afterClosingBracket) {
                formatted += '\n';
                formatted.append(indent * 2, ' ');
            }
            
            if (i + 1 < xml.size() && xml[i + 1] == '/') {
                indent--;
                formatted += '\n';
                formatted.append(indent * 2, ' ');
            }
            
            inTag = true;
            afterClosingBracket = false;
        }
        
        formatted += c;
        
        if (c == '>') {
            inTag = false;
            afterClosingBracket = true;
            
            if (i > 0 && xml[i - 1] != '/') {
                if (i + 1 < xml.size() && xml[i + 1] != '<') {
                    // Don't add newline if followed by text content
                } else {
                    indent++;
                }
            }
        }
    }
    
    return formatted;
}

std::map<std::string, std::string> ODFInspector::parseMetadata() const {
    std::map<std::string, std::string> metadata;
    
    // Simple tag extraction (not full XML parsing)
    auto extractTag = [](const std::string& xml, const std::string& tagName) -> std::string {
        std::string openTag = "<" + tagName + ">";
        std::string closeTag = "</" + tagName + ">";
        
        size_t start = xml.find(openTag);
        if (start == std::string::npos) return "";
        
        start += openTag.length();
        size_t end = xml.find(closeTag, start);
        if (end == std::string::npos) return "";
        
        return xml.substr(start, end - start);
    };
    
    metadata["Title"] = extractTag(metaXml_, "dc:title");
    metadata["Creator"] = extractTag(metaXml_, "dc:creator");
    metadata["Date"] = extractTag(metaXml_, "dc:date");
    metadata["Generator"] = extractTag(metaXml_, "meta:generator");
    
    // Remove empty entries
    for (auto it = metadata.begin(); it != metadata.end();) {
        if (it->second.empty()) {
            it = metadata.erase(it);
        } else {
            ++it;
        }
    }
    
    return metadata;
}

std::string ODFInspector::getDocTypeFromMime(const std::string& mime) const {
    if (mime.find("text") != std::string::npos) {
        return "Text Document (.odt)";
    } else if (mime.find("spreadsheet") != std::string::npos) {
        return "Spreadsheet (.ods)";
    } else if (mime.find("presentation") != std::string::npos) {
        return "Presentation (.odp)";
    } else if (mime.find("graphics") != std::string::npos) {
        return "Drawing (.odg)";
    } else if (mime.find("chart") != std::string::npos) {
        return "Chart (.odc)";
    } else if (mime.find("formula") != std::string::npos) {
        return "Formula (.odf)";
    }
    return "Unknown ODF Document";
}
