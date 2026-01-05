#ifndef ODFINSPECTOR_H
#define ODFINSPECTOR_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "ZipReader.h"

/**
 * @brief Main class for inspecting ODF (Open Document Format) files
 * 
 * This class provides functionality to:
 * - Parse ODF structure
 * - Extract and display metadata
 * - Show document content
 * - List embedded resources
 */
class ODFInspector {
public:
    /**
     * @brief Construct a new ODF Inspector object
     * @param odfPath Path to the ODF file
     */
    explicit ODFInspector(const std::string& odfPath);

    /**
     * @brief Destroy the ODF Inspector object
     */
    ~ODFInspector();

    /**
     * @brief Load and validate the ODF file
     * @return true if successful, false otherwise
     */
    bool load();

    /**
     * @brief Display a summary of the ODF file
     */
    void displaySummary() const;

    /**
     * @brief Display the file structure
     */
    void displayStructure() const;

    /**
     * @brief Display metadata from meta.xml
     */
    void displayMetadata() const;

    /**
     * @brief Display content preview from content.xml
     */
    void displayContent() const;

    /**
     * @brief Display styles information from styles.xml
     */
    void displayStyles() const;

    /**
     * @brief Display the manifest file
     */
    void displayManifest() const;

    /**
     * @brief Extract a specific file and display its content
     * @param filename Name of the file to extract
     */
    void displayFile(const std::string& filename) const;

    /**
     * @brief List all embedded images
     */
    void listImages() const;

    /**
     * @brief Get the MIME type of the document
     * @return MIME type string
     */
    std::string getMimeType() const;

    /**
     * @brief Get the document type (text, spreadsheet, presentation, etc.)
     * @return Document type string
     */
    std::string getDocumentType() const;

    /**
     * @brief Check if the file is a valid ODF document
     * @return true if valid, false otherwise
     */
    bool isValidODF() const;

    /**
     * @brief Get the last error message
     * @return Error message string
     */
    std::string getLastError() const;

private:
    std::string odfPath_;
    std::unique_ptr<ZipReader> zipReader_;
    std::string mimeType_;
    mutable std::string lastError_;
    bool isLoaded_;

    // Key ODF file contents
    std::string contentXml_;
    std::string metaXml_;
    std::string stylesXml_;
    std::string manifestXml_;

    // Helper methods
    bool validateODF();
    bool extractCoreFiles();
    std::string formatXML(const std::string& xml) const;
    std::string extractTextFromXML(const std::string& xml) const;
    std::map<std::string, std::string> parseMetadata() const;
    std::string getDocTypeFromMime(const std::string& mime) const;
};

#endif // ODFINSPECTOR_H
