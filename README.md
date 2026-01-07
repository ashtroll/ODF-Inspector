# ODF Inspector

A C++ tool for inspecting Open Document Format (ODF) files, useful for LibreOffice development and debugging.

## What is ODF?

ODF (Open Document Format) is an open standard for office documents (text, spreadsheets, presentations). Files include:
- `.odt` - Text documents
- `.ods` - Spreadsheets  
- `.odp` - Presentations
- `.odg` - Graphics

ODF files are ZIP archives containing XML files and resources.

## Features

- Extract and list all files in an ODF archive
- Display XML content with pretty formatting
- Show document metadata (content.xml, meta.xml, styles.xml)
- Inspect manifest and document structure
- Useful for understanding ODF structure before contributing to LibreOffice

## Building

### Prerequisites

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.15 or higher
- zlib library
- libxml2 library

#### Windows (with vcpkg)
```bash
vcpkg install zlib libxml2
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

#### Linux/macOS
```bash
# Install dependencies
# Ubuntu/Debian:
sudo apt-get install zlib1g-dev libxml2-dev

# macOS:
brew install zlib libxml2

# Build
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./odf-inspector <path-to-odf-file>
```

Examples:
```bash
./odf-inspector document.odt
./odf-inspector spreadsheet.ods
./odf-inspector presentation.odp
```

## Project Structure

```
odf-inspector/
├── include/           # Header files
│   ├── ODFInspector.h
│   └── ZipReader.h
├── src/              # Implementation files
│   ├── main.cpp
│   ├── ODFInspector.cpp
│   └── ZipReader.cpp
├── CMakeLists.txt    # Build configuration
└── README.md
```

## How It Works

1. **ZIP Extraction**: ODF files are ZIP archives, so we use zlib to extract contents
2. **XML Parsing**: Extract and parse key XML files (content.xml, meta.xml, styles.xml)
3. **Display**: Pretty-print the structure and metadata

## Key ODF Files

- `mimetype` - Identifies the ODF document type
- `content.xml` - Main document content
- `meta.xml` - Document metadata (author, date, etc.)
- `styles.xml` - Style definitions
- `settings.xml` - Application settings
- `META-INF/manifest.xml` - File manifest
- `Pictures/` - Embedded images

## Contributing to LibreOffice

This tool helps you:
- Understand ODF file structure
- Debug document issues
- Test file generation
- Learn the format before contributing to LibreOffice core

## Resources

- [ODF Specification](http://docs.oasis-open.org/office/v1.2/OpenDocument-v1.2.pdf)
- [LibreOffice Development](https://wiki.documentfoundation.org/Development)
- [LibreOffice Core Repository](https://github.com/LibreOffice/core)

## License

MIT License - Feel free to use and modify for your LibreOffice contribution work!..

