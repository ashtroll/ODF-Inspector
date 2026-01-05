#include <iostream>
#include <string>
#include <memory>
#include "ODFInspector.h"

void printUsage(const char* programName) {
    std::cout << "\nODF Inspector - Inspect Open Document Format files\n";
    std::cout << "===================================================\n\n";
    std::cout << "Usage: " << programName << " <odf-file> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --summary      Display document summary (default)\n";
    std::cout << "  --structure    List all files in the archive\n";
    std::cout << "  --metadata     Show document metadata\n";
    std::cout << "  --content      Display content.xml preview\n";
    std::cout << "  --styles       Display styles.xml preview\n";
    std::cout << "  --manifest     Display manifest file\n";
    std::cout << "  --images       List embedded images\n";
    std::cout << "  --all          Display everything\n";
    std::cout << "  --file <name>  Extract and display specific file\n";
    std::cout << "  --help         Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " document.odt\n";
    std::cout << "  " << programName << " spreadsheet.ods --all\n";
    std::cout << "  " << programName << " presentation.odp --metadata --content\n";
    std::cout << "  " << programName << " document.odt --file content.xml\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string odfPath = argv[1];

    // Check for help flag
    if (odfPath == "--help" || odfPath == "-h") {
        printUsage(argv[0]);
        return 0;
    }

    // Parse options
    bool showSummary = true;
    bool showStructure = false;
    bool showMetadata = false;
    bool showContent = false;
    bool showStyles = false;
    bool showManifest = false;
    bool showImages = false;
    bool showAll = false;
    std::string specificFile;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--summary") {
            showSummary = true;
        } else if (arg == "--structure") {
            showStructure = true;
        } else if (arg == "--metadata") {
            showMetadata = true;
        } else if (arg == "--content") {
            showContent = true;
        } else if (arg == "--styles") {
            showStyles = true;
        } else if (arg == "--manifest") {
            showManifest = true;
        } else if (arg == "--images") {
            showImages = true;
        } else if (arg == "--all") {
            showAll = true;
        } else if (arg == "--file" && i + 1 < argc) {
            specificFile = argv[++i];
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    // If --all is specified, enable everything
    if (showAll) {
        showSummary = true;
        showStructure = true;
        showMetadata = true;
        showContent = true;
        showStyles = true;
        showManifest = true;
        showImages = true;
    }

    // Create inspector and load the file
    std::cout << "Loading ODF file: " << odfPath << "\n";
    
    auto inspector = std::make_unique<ODFInspector>(odfPath);
    
    if (!inspector->load()) {
        std::cerr << "Error: " << inspector->getLastError() << "\n";
        return 1;
    }

    std::cout << "Successfully loaded ODF file!\n";

    // Display requested information
    if (showSummary) {
        inspector->displaySummary();
    }

    if (showStructure) {
        inspector->displayStructure();
    }

    if (showMetadata) {
        inspector->displayMetadata();
    }

    if (showContent) {
        inspector->displayContent();
    }

    if (showStyles) {
        inspector->displayStyles();
    }

    if (showManifest) {
        inspector->displayManifest();
    }

    if (showImages) {
        inspector->listImages();
    }

    if (!specificFile.empty()) {
        inspector->displayFile(specificFile);
    }

    std::cout << "\nInspection complete!\n";
    return 0;
}
