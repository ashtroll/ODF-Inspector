# Building the ODF Inspector

## Quick Start (Windows with vcpkg)

### Step 1: Install vcpkg
```powershell
# Clone vcpkg if you don't have it
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### Step 2: Install Dependencies
```powershell
# Install required libraries
.\vcpkg install zlib:x64-windows
.\vcpkg install libxml2:x64-windows
```

### Step 3: Build the Project
```powershell
# Navigate to your project directory
cd "c:\Users\msi\Desktop\odf inspector"

# Configure with CMake
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build --config Release
```

### Step 4: Run
```powershell
.\build\Release\odf-inspector.exe document.odt
```

## Alternative: Using MinGW on Windows

If you prefer MinGW:

```powershell
# Install dependencies via pacman (if using MSYS2)
pacman -S mingw-w64-x86_64-zlib mingw-w64-x86_64-libxml2

# Build
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## Linux Build

```bash
# Install dependencies
sudo apt-get install build-essential cmake zlib1g-dev libxml2-dev

# Build
mkdir build && cd build
cmake ..
make

# Run
./odf-inspector document.odt
```

## macOS Build

```bash
# Install dependencies
brew install cmake zlib libxml2

# Build
mkdir build && cd build
cmake ..
make

# Run
./odf-inspector document.odt
```

## Testing

Create a simple test document in LibreOffice and save it as .odt, .ods, or .odp.
Then run:

```bash
./odf-inspector test.odt --all
```

This will show you the complete structure of the ODF file!

## Troubleshooting

### minizip not found
If you get errors about minizip headers, you may need to install it separately:

**vcpkg:**
```powershell
.\vcpkg install minizip:x64-windows
```

**Linux:**
```bash
sudo apt-get install libminizip-dev
```

**macOS:**
```bash
brew install minizip
```

Then update the CMakeLists.txt to find minizip explicitly.

### CMake can't find libraries
Make sure you're using the vcpkg toolchain file:
```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
```

## Next Steps for LibreOffice Contribution

1. **Study ODF Structure**: Use this tool to inspect various ODF files
2. **Read the Spec**: Check out the ODF specification (linked in main README)
3. **Explore LibreOffice Code**: Clone the LibreOffice core repository
4. **Join the Community**: Visit https://www.libreoffice.org/community/developers/
5. **Find Easy Hacks**: Look for "Easy Hack" issues to start contributing

Happy coding and welcome to the LibreOffice community! 🎉
