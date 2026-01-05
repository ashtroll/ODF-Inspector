#include "ZipReader.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <zlib.h>

// For minizip
#ifdef _WIN32
    #include <contrib/minizip/unzip.h>
#else
    #include <minizip/unzip.h>
#endif

ZipReader::ZipReader(const std::string& zipPath)
    : zipPath_(zipPath)
    , zipHandle_(nullptr)
    , isOpen_(false) {
}

ZipReader::~ZipReader() {
    close();
}

bool ZipReader::open() {
    if (isOpen_) {
        return true;
    }

    zipHandle_ = unzOpen(zipPath_.c_str());
    if (zipHandle_ == nullptr) {
        lastError_ = "Failed to open ZIP file: " + zipPath_;
        return false;
    }

    isOpen_ = true;
    return true;
}

void ZipReader::close() {
    if (zipHandle_ != nullptr) {
        unzClose(static_cast<unzFile>(zipHandle_));
        zipHandle_ = nullptr;
        isOpen_ = false;
    }
}

bool ZipReader::isOpen() const {
    return isOpen_;
}

std::vector<std::string> ZipReader::listFiles() const {
    std::vector<std::string> files;
    
    if (!isOpen_) {
        return files;
    }

    unzFile uf = static_cast<unzFile>(zipHandle_);
    
    if (unzGoToFirstFile(uf) != UNZ_OK) {
        return files;
    }

    do {
        char filename[256];
        unz_file_info fileInfo;
        
        if (unzGetCurrentFileInfo(uf, &fileInfo, filename, sizeof(filename),
                                   nullptr, 0, nullptr, 0) == UNZ_OK) {
            files.push_back(filename);
        }
    } while (unzGoToNextFile(uf) == UNZ_OK);

    return files;
}

std::string ZipReader::extractFile(const std::string& filename) const {
    if (!isOpen_) {
        lastError_ = "ZIP file is not open";
        return "";
    }

    unzFile uf = static_cast<unzFile>(zipHandle_);
    
    if (unzLocateFile(uf, filename.c_str(), 0) != UNZ_OK) {
        lastError_ = "File not found in archive: " + filename;
        return "";
    }

    unz_file_info fileInfo;
    if (unzGetCurrentFileInfo(uf, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
        lastError_ = "Failed to get file info: " + filename;
        return "";
    }

    if (unzOpenCurrentFile(uf) != UNZ_OK) {
        lastError_ = "Failed to open file: " + filename;
        return "";
    }

    std::vector<char> buffer(fileInfo.uncompressed_size);
    int bytesRead = unzReadCurrentFile(uf, buffer.data(), buffer.size());
    unzCloseCurrentFile(uf);

    if (bytesRead < 0) {
        lastError_ = "Failed to read file: " + filename;
        return "";
    }

    return std::string(buffer.begin(), buffer.end());
}

bool ZipReader::fileExists(const std::string& filename) const {
    if (!isOpen_) {
        return false;
    }

    unzFile uf = static_cast<unzFile>(zipHandle_);
    return unzLocateFile(uf, filename.c_str(), 0) == UNZ_OK;
}

size_t ZipReader::getFileSize(const std::string& filename) const {
    if (!isOpen_) {
        return 0;
    }

    unzFile uf = static_cast<unzFile>(zipHandle_);
    
    if (unzLocateFile(uf, filename.c_str(), 0) != UNZ_OK) {
        return 0;
    }

    unz_file_info fileInfo;
    if (unzGetCurrentFileInfo(uf, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
        return 0;
    }

    return fileInfo.uncompressed_size;
}

std::string ZipReader::getLastError() const {
    return lastError_;
}
