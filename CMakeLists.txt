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
set(SOURCES
    src/main.cpp
    src/ODFInspector.cpp
    src/ZipReader.cpp
)

# Headers
set(HEADERS
    include/ODFInspector.h
    include/ZipReader.h
)

# Create executable
add_executable(odf-inspector ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(odf-inspector 
    ZLIB::ZLIB
    unofficial::minizip::minizip
)

if(LibXml2_FOUND)
    target_link_libraries(odf-inspector ${LIBXML2_LIBRARIES})
    target_compile_definitions(odf-inspector PRIVATE HAVE_LIBXML2)
endif()

# Installation
install(TARGETS odf-inspector DESTINATION bin)

# Enable warnings
if(MSVC)
    target_compile_options(odf-inspector PRIVATE /W4)
else()
    target_compile_options(odf-inspector PRIVATE -Wall -Wextra -pedantic)
endif()
