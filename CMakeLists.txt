cmake_minimum_required(VERSION 3.15)
project(ODFInspector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required libraries
find_package(ZLIB REQUIRED)
find_package(LibXml2 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LIBXML2_INCLUDE_DIR}
)

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
    ${LIBXML2_LIBRARIES}
)

# Installation
install(TARGETS odf-inspector DESTINATION bin)

# Enable warnings
if(MSVC)
    target_compile_options(odf-inspector PRIVATE /W4)
else()
    target_compile_options(odf-inspector PRIVATE -Wall -Wextra -pedantic)
endif()
