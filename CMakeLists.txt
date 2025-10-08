cmake_minimum_required(VERSION 3.15)
project(ColorImageGenerator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform-specific settings
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# Platform-specific libraries for screen resolution detection
if(UNIX AND NOT APPLE)
    # Linux: X11
    find_package(X11 REQUIRED)
    set(PLATFORM_LIBS ${X11_LIBRARIES})
    set(PLATFORM_INCLUDES ${X11_INCLUDE_DIR})
elseif(APPLE)
    # macOS: ApplicationServices framework
    find_library(APP_SERVICES ApplicationServices)
    set(PLATFORM_LIBS ${APP_SERVICES})
elseif(WIN32)
    # Windows: Built-in libraries
    set(PLATFORM_LIBS "")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PLATFORM_INCLUDES}
)

# Source files
set(SOURCES
    src/main.cpp
    src/Color.cpp
    src/Resolution.cpp
    src/ImageWriter.cpp
    src/formats/STBImageWriter.cpp
)

# Header files (for IDE organization)
set(HEADERS
    include/Color.hpp
    include/Resolution.hpp
    include/ImageFormat.hpp
    include/ImageWriter.hpp
    include/formats/STBImageWriter.hpp
    include/stb_image_write.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries (only platform-specific libs for screen detection)
target_link_libraries(${PROJECT_NAME}
    ${PLATFORM_LIBS}
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "")
message(STATUS "===== Configuration Summary =====")
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Image library: stb_image_write.h (header-only, no external deps)")
if(PLATFORM_LIBS)
    message(STATUS "Platform libraries: ${PLATFORM_LIBS}")
endif()
message(STATUS "=================================")
message(STATUS "")
