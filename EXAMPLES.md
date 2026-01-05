# ODF Inspector Examples

## Basic Usage

### Inspect a text document
```bash
odf-inspector document.odt
```
Output:
```
========================================
ODF INSPECTOR SUMMARY
========================================

File: document.odt
Type: Text Document (.odt)
MIME: application/vnd.oasis.opendocument.text
Valid ODF: Yes

Total files in archive: 12

Core Files:
  - content.xml: Present
  - meta.xml: Present
  - styles.xml: Present
  - manifest.xml: Present
========================================
```

### View file structure
```bash
odf-inspector document.odt --structure
```
Shows all files in the ZIP archive with their sizes.

### View metadata
```bash
odf-inspector document.odt --metadata
```
Output:
```
========================================
METADATA (meta.xml)
========================================

  Title: My Document
  Creator: John Doe
  Date: 2026-01-05T10:30:00
  Generator: LibreOffice/7.6
========================================
```

### View content preview
```bash
odf-inspector document.odt --content
```
Shows a preview of the content.xml file (main document content).

### Extract specific file
```bash
odf-inspector document.odt --file content.xml
```
Extracts and displays any file from the archive.

### View everything
```bash
odf-inspector document.odt --all
```
Displays all available information about the document.

## Advanced Usage

### Inspect spreadsheet
```bash
odf-inspector budget.ods --metadata --content
```

### List embedded images
```bash
odf-inspector presentation.odp --images
```

### Inspect manifest
```bash
odf-inspector document.odt --manifest
```
Shows the META-INF/manifest.xml which lists all files in the document.

## Understanding ODF Structure

An ODF file is a ZIP archive containing:

```
document.odt
├── mimetype                    # Document type identifier
├── META-INF/
│   └── manifest.xml           # File manifest
├── content.xml                # Main document content
├── meta.xml                   # Metadata (author, date, etc.)
├── styles.xml                 # Style definitions
├── settings.xml               # Application settings
├── Configurations2/           # UI configuration
│   ├── accelerator/
│   ├── menubar/
│   └── toolbar/
└── Pictures/                  # Embedded images
    └── image1.png
```

## Use Cases for LibreOffice Development

### 1. Debug File Generation Issues
When LibreOffice generates incorrect ODF files:
```bash
odf-inspector broken.odt --all > output.txt
```
Examine the structure to find issues.

### 2. Compare Documents
```bash
odf-inspector original.odt --content > original.txt
odf-inspector modified.odt --content > modified.txt
diff original.txt modified.txt
```

### 3. Test Style Changes
```bash
odf-inspector document.odt --styles
```
See how styles are encoded in the XML.

### 4. Verify Metadata
```bash
odf-inspector document.odt --metadata
```
Check if metadata is correctly set.

### 5. Inspect Embedded Resources
```bash
odf-inspector presentation.odp --images
```
List all embedded images and their paths.

## Creating Test Documents

1. Open LibreOffice Writer
2. Create a simple document with:
   - Some text
   - Bold/italic formatting
   - An embedded image
   - A table
3. Save as `test.odt`
4. Inspect it:
```bash
odf-inspector test.odt --all
```

## Integration with LibreOffice Development

### Workflow Example:

1. **Make code change** in LibreOffice core
2. **Build LibreOffice**
3. **Create test document** with your new feature
4. **Inspect with odf-inspector** to verify correct XML generation
5. **Compare** with expected ODF structure
6. **Submit patch** to LibreOffice

### Useful Files to Inspect:

- **content.xml**: Document structure and content
- **styles.xml**: All styles (paragraph, character, page)
- **meta.xml**: Document properties
- **settings.xml**: View settings, print settings
- **manifest.xml**: List of all files and their MIME types

## Tips for LibreOffice Contributors

1. **Learn XML namespaces**: ODF uses many namespaces (text:, style:, fo:, etc.)
2. **Study the spec**: ODF specification explains each element
3. **Compare with MSOffice**: Use this tool to compare ODF vs OOXML structure
4. **Test edge cases**: Empty documents, large documents, complex formatting
5. **Validate**: Ensure generated XML is valid ODF

## Resources

- [ODF Specification](http://docs.oasis-open.org/office/v1.2/OpenDocument-v1.2.pdf)
- [LibreOffice Developer Guide](https://wiki.documentfoundation.org/Development)
- [Easy Hacks](https://wiki.documentfoundation.org/Development/Easy_Hacks)
- [LibreOffice Gerrit](https://gerrit.libreoffice.org/)

Happy inspecting! 🔍
