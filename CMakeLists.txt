cmake_minimum_required(VERSION 3.15)
project(ODFInspector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required libraries
find_package(ZLIB REQUIRED)
find_package(unofficial-minizip CONFIG REQUIRED)

# Try to find libxml2 - make it optional for now
find_package(LibXml2)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIRS}
)

if(LibXml2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIR})
endif()

# Source files
set(CLI_SOURCES
    src/main.cpp
    src/ODFInspector.cpp
    src/ZipReader.cpp
)

set(GUI_SOURCES
    src/gui_main.cpp
    src/ODFInspector.cpp
    src/ZipReader.cpp
)

# Headers
set(HEADERS
    include/ODFInspector.h
    include/ZipReader.h
)

# Create CLI executable
add_executable(odf-inspector ${CLI_SOURCES} ${HEADERS})

# Create GUI executable
add_executable(odf-inspector-gui WIN32 ${GUI_SOURCES} ${HEADERS})

# Link libraries for CLI
target_link_libraries(odf-inspector 
    ZLIB::ZLIB
    unofficial::minizip::minizip
)

# Link libraries for GUI
target_link_libraries(odf-inspector-gui
    ZLIB::ZLIB
    unofficial::minizip::minizip
    comctl32
)

if(LibXml2_FOUND)
    target_link_libraries(odf-inspector ${LIBXML2_LIBRARIES})
    target_compile_definitions(odf-inspector PRIVATE HAVE_LIBXML2)
    
    target_link_libraries(odf-inspector-gui ${LIBXML2_LIBRARIES})
    target_compile_definitions(odf-inspector-gui PRIVATE HAVE_LIBXML2)
endif()

# Installation
install(TARGETS odf-inspector odf-inspector-gui DESTINATION bin)

# Enable warnings
if(MSVC)
    target_compile_options(odf-inspector PRIVATE /W4)
else()
    target_compile_options(odf-inspector PRIVATE -Wall -Wextra -pedantic)
endif()
