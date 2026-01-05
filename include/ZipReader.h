#ifndef ZIPREADER_H
#define ZIPREADER_H

#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * @brief Handles reading and extracting files from ZIP archives
 * 
 * ODF files are ZIP archives, so this class provides functionality
 * to extract and read individual files from within the archive.
 */
class ZipReader {
public:
    /**
     * @brief Construct a new Zip Reader object
     * @param zipPath Path to the ZIP/ODF file
     */
    explicit ZipReader(const std::string& zipPath);
    
    /**
     * @brief Destroy the Zip Reader object
     */
    ~ZipReader();

    /**
     * @brief Open the ZIP archive for reading
     * @return true if successful, false otherwise
     */
    bool open();

    /**
     * @brief Close the ZIP archive
     */
    void close();

    /**
     * @brief Check if the archive is currently open
     * @return true if open, false otherwise
     */
    bool isOpen() const;

    /**
     * @brief List all files in the archive
     * @return Vector of file paths within the archive
     */
    std::vector<std::string> listFiles() const;

    /**
     * @brief Extract a specific file from the archive
     * @param filename Name of the file to extract
     * @return Content of the file as a string
     */
    std::string extractFile(const std::string& filename) const;

    /**
     * @brief Check if a file exists in the archive
     * @param filename Name of the file to check
     * @return true if file exists, false otherwise
     */
    bool fileExists(const std::string& filename) const;

    /**
     * @brief Get the size of a file in the archive
     * @param filename Name of the file
     * @return Size in bytes, or 0 if file doesn't exist
     */
    size_t getFileSize(const std::string& filename) const;

    /**
     * @brief Get the last error message
     * @return Error message string
     */
    std::string getLastError() const;

private:
    std::string zipPath_;
    void* zipHandle_;  // Platform-specific ZIP handle
    mutable std::string lastError_;
    bool isOpen_;

    // Helper methods for ZIP handling
    bool readZipEntry(const std::string& filename, std::vector<char>& buffer) const;
};

#endif // ZIPREADER_H
