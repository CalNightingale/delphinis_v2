cmake_minimum_required(VERSION 3.15)
project(Delphinis VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Dependencies
find_package(OpenGL REQUIRED)

# GLFW - using FetchContent for easy cross-platform setup
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# GLEW - OpenGL loader (simpler alternative to GLAD)
find_package(GLEW REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
)

# Main executable
add_executable(delphinis
    src/main.cpp
)

target_link_libraries(delphinis
    glfw
    GLEW::GLEW
    ${OPENGL_LIBRARIES}
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(delphinis
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(delphinis PRIVATE /W4)
else()
    target_compile_options(delphinis PRIVATE -Wall -Wextra -Wpedantic)
endif()
