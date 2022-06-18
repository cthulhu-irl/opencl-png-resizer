cmake_minimum_required(VERSION 3.8)

project(png_resizer CXX)

set(CMAKE_CXX_STANDARD 17)

include("cmake/setup_vcpkg.cmake")

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_executable(${PROJECT_NAME} src/main.cpp src/pngfile.cpp src/openclmanager.cpp)

add_subdirectory("third_party/zlib")

set(PNG_BUILD_ZLIB ON CACHE BOOL "" FORCE)

set(PNG_TESTS OFF CACHE BOOL "")
set(PNG_EXECUTABLES OFF CACHE BOOL "")
set(PNG_SHARED OFF CACHE BOOL "")
set(PNG_STATIC ON CACHE BOOL "")

add_subdirectory("third_party/libpng")

target_include_directories(${PROJECT_NAME} PRIVATE png)
target_link_libraries(${PROJECT_NAME} PRIVATE png_static)

find_package(OpenCL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)

