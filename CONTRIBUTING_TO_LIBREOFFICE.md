# Contributing to LibreOffice - Getting Started

This guide will help you use the ODF Inspector as a stepping stone to contributing to LibreOffice.

## Understanding ODF (Open Document Format)

ODF is an international standard (ISO/IEC 26300) for office documents. LibreOffice is the primary implementation of this standard.

### Key Concepts:

1. **ZIP Archive**: Every ODF file is a ZIP archive
2. **XML Files**: Document structure is defined in XML
3. **Namespaces**: ODF uses XML namespaces for different elements
4. **MIME Types**: Each document type has a specific MIME type

## ODF Structure Deep Dive

### Document Types:
- **Text** (.odt): `application/vnd.oasis.opendocument.text`
- **Spreadsheet** (.ods): `application/vnd.oasis.opendocument.spreadsheet`
- **Presentation** (.odp): `application/vnd.oasis.opendocument.presentation`
- **Drawing** (.odg): `application/vnd.oasis.opendocument.graphics`

### Core XML Files:

#### 1. content.xml
The main document content. Contains:
- Document body
- Automatic styles (styles generated on-the-fly)
- Font declarations
- Actual content (text, tables, etc.)

Example structure:
```xml
<office:document-content>
  <office:scripts/>
  <office:font-face-decls/>
  <office:automatic-styles/>
  <office:body>
    <office:text>
      <text:p text:style-name="P1">Hello World</text:p>
    </office:text>
  </office:body>
</office:document-content>
```

#### 2. styles.xml
Style definitions:
- Master pages
- Page layouts
- Paragraph styles
- Character styles
- List styles

#### 3. meta.xml
Document metadata:
- Title, author, description
- Creation/modification dates
- Document statistics (word count, page count)
- Keywords

#### 4. settings.xml
Application-specific settings:
- View settings (zoom level, visible grids)
- Print settings
- Window positions

#### 5. manifest.xml
File manifest listing all files in the archive with their MIME types.

## Using ODF Inspector for LibreOffice Development

### Workflow 1: Understanding Existing Features

1. **Create a document** in LibreOffice with a specific feature
   ```bash
   # E.g., create a document with bold text, tables, and images
   ```

2. **Inspect it**
   ```bash
   odf-inspector document.odt --all > analysis.txt
   ```

3. **Study the XML** to see how LibreOffice implements the feature

### Workflow 2: Testing Your Changes

When you modify LibreOffice code:

1. **Build your modified LibreOffice**
   ```bash
   cd libreoffice-core
   make
   ```

2. **Create test document** with your modified version

3. **Inspect the generated ODF**
   ```bash
   odf-inspector test.odt --content
   ```

4. **Verify XML matches ODF specification**

### Workflow 3: Bug Investigation

When investigating a bug:

1. **Get the problematic document**
2. **Inspect its structure**
   ```bash
   odf-inspector buggy.odt --all
   ```
3. **Look for malformed XML or incorrect structures**
4. **Find the LibreOffice code** that generates that part
5. **Fix and test**

## Common ODF Patterns

### Text Formatting
```xml
<text:p text:style-name="P1">
  <text:span text:style-name="T1">Bold text</text:span>
</text:p>
```

### Tables
```xml
<table:table table:name="Table1">
  <table:table-column/>
  <table:table-row>
    <table:table-cell>
      <text:p>Cell content</text:p>
    </table:table-cell>
  </table:table-row>
</table:table>
```

### Images
```xml
<draw:frame>
  <draw:image xlink:href="Pictures/image1.png"/>
</draw:frame>
```

## LibreOffice Code Structure

Key directories in LibreOffice core:

- **sw/**: Writer (text documents)
- **sc/**: Calc (spreadsheets)
- **sd/**: Impress/Draw (presentations/drawings)
- **xmloff/**: XML import/export code
- **oox/**: OOXML (MS Office) import/export
- **filter/**: File format filters

## Getting Started with LibreOffice Development

### 1. Set Up Development Environment

```bash
# Clone LibreOffice
git clone https://git.libreoffice.org/core libreoffice-core
cd libreoffice-core

# Install dependencies (Ubuntu/Debian)
sudo apt-get build-dep libreoffice

# Configure
./autogen.sh --enable-dbgutil

# Build (this takes a while!)
make
```

### 2. Find an Easy Hack

Visit: https://wiki.documentfoundation.org/Development/Easy_Hacks

Look for issues tagged with:
- **Difficulty: Beginner**
- **Skill: C++**
- Keywords: ODF, XML, import, export

### 3. Understand the Code Flow

For ODF export in Writer:
```
sw/source/filter/xml/
├── xmlexp.cxx     # Main export class
├── xmltext.cxx    # Text export
├── xmltexte.cxx   # Text elements export
└── xmlfmt.cxx     # Format export
```

For ODF import:
```
xmloff/source/text/
├── txtimp.cxx     # Text import
└── txtparae.cxx   # Paragraph import
```

### 4. Make Your First Contribution

Example: Fix a simple ODF export bug

1. **Identify the bug** (e.g., incorrect XML attribute)
2. **Find the code** that generates that XML
3. **Make the fix**
4. **Test** with ODF Inspector
5. **Create unit test**
6. **Submit patch** to Gerrit

## Useful ODF Inspector Commands for Development

### Before coding:
```bash
# Understand current behavior
odf-inspector reference.odt --content > before.xml
```

### After your changes:
```bash
# Test your changes
odf-inspector test.odt --content > after.xml
diff before.xml after.xml
```

### Validate structure:
```bash
# Check all core files are present
odf-inspector test.odt --summary
```

### Debug specific features:
```bash
# If working on styles
odf-inspector test.odt --styles

# If working on metadata
odf-inspector test.odt --metadata
```

## Learning Resources

### Essential Reading:
1. **ODF Specification**: http://docs.oasis-open.org/office/OpenDocument/v1.2/
2. **LibreOffice Dev Docs**: https://docs.libreoffice.org/
3. **API Reference**: https://api.libreoffice.org/

### Community:
- **IRC**: #libreoffice-dev on irc.libera.chat
- **Mailing List**: libreoffice@lists.freedesktop.org
- **Forum**: https://ask.libreoffice.org/

### Code Review:
- **Gerrit**: https://gerrit.libreoffice.org/
- **OpenGrok**: https://opengrok.libreoffice.org/

## Tips for Success

1. **Start Small**: Fix typos, improve error messages, add comments
2. **Read Code**: Spend time reading existing code before writing
3. **Use ODF Inspector**: Always inspect your generated ODF files
4. **Test Thoroughly**: Create comprehensive test cases
5. **Ask Questions**: The community is friendly and helpful
6. **Be Patient**: Building LibreOffice takes time, debugging takes time
7. **Read the Spec**: When in doubt, check the ODF specification

## Common Contribution Areas

### 1. ODF Import/Export
- Fix XML generation bugs
- Add support for new ODF features
- Improve compatibility

### 2. File Format Filters
- Improve DOCX/XLSX import
- Fix formatting issues
- Add missing feature support

### 3. UI/UX
- Add new dialog options
- Improve existing dialogs
- Accessibility improvements

### 4. Performance
- Optimize slow operations
- Reduce memory usage
- Improve startup time

### 5. Quality Assurance
- Write unit tests
- Add integration tests
- Improve error handling

## Next Steps

1. ✅ **Build this ODF Inspector**
2. ✅ **Create test ODF documents**
3. ✅ **Study their structure**
4. 📚 **Read the ODF specification**
5. 💻 **Clone LibreOffice core**
6. 🔨 **Build LibreOffice**
7. 🐛 **Pick an Easy Hack**
8. 🎯 **Make your first contribution**

Good luck with your LibreOffice contribution journey! The community welcomes you! 🚀

## Quick Reference

### XML Namespaces in ODF:
- `office:` - Document structure
- `text:` - Text content
- `table:` - Tables
- `draw:` - Graphics and frames
- `style:` - Style definitions
- `fo:` - Formatting properties (from XSL-FO)
- `svg:` - SVG attributes
- `xlink:` - Links

### Essential Tools:
- **odf-inspector**: Inspect ODF files (this project!)
- **xmllint**: Validate XML
- **unzip**: Extract ODF manually
- **diffuse/meld**: Compare XML files
- **gdb**: Debug LibreOffice

### Helpful Commands:
```bash
# Extract ODF manually
unzip document.odt -d extracted/

# Pretty-print XML
xmllint --format content.xml

# Validate against schema
xmllint --schema OpenDocument-v1.2-schema.rng content.xml

# Search LibreOffice code
git grep "text:style-name"
```

Happy hacking! 🎉
