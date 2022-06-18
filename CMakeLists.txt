cmake_minimum_required(VERSION 3.8)

project(png_resizer CXX)

set(CMAKE_CXX_STANDARD 17)

include("cmake/setup_vcpkg.cmake")

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

set(RESIZER_SOURCES LIST src/main.cpp src/pngfile.cpp src/openclmanager.cpp src/resizer.cpp)

add_executable(${PROJECT_NAME} src/main.cpp src/pngfile.cpp src/resizer.cpp)

target_sources(${PROJECT_NAME} PRIVATE src/main.cpp src/pngfile.cpp src/resizer.cpp)

set(PNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng/include")
set(PNG_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng/lib/libpng16_static.lib")

target_include_directories(${PROJECT_NAME} PRIVATE ${PNG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PNG_LIBRARY})

find_package(OpenCL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)
