# OpenCL PNG Resizer

Resizes a given png file to given dimensions using libpng and opencl as backend, with nearest neighbour interpolation algorithm.

## Build

clone and enter the directory, then:
```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

the resulting executable can be found at `./bin/Release/png_resizer` (`.exe` on windows).
example:
```
./bin/Release/png_resizer photo.png output.png 300 200
```

## Reproducing `third_party` (Windows, bash or similar)

libpng requires zlib which we can get and compile as follow, considering current directory being `third_party`:
```
git clone https://github.com/madler/zlib zlibsrc
cd zlibsrc

mkdir build
cd build
cmake ..
cmake --build . --config Release

cd ../../
mkdir -p zlib/include zlib/lib

cp zlibsrc/build/Release/zlibstatic.lib zlib/lib/
cp zlibsrc/build/zconf.h zlib/include/
cp zlibsrc/zlib.h zlib/include/

rm -rf zlibsrc
```

now that we have zlib's static library and public header files, we can go on building libpng:
```
git clone https://github.com/glennrp/libpng.git libpngsrc
cd libpng

mkdir build
cd build
cmake .. -DZLIB_LIBRARY=../../zlib/lib/zlibstatic -DZLIB_INCLUDE_DIR=../../zlib/include
cmake --build . --config Release

cd ../../
mkdir -p libpng/lib libpng/include

cp libpngsrc/build/Release/libpng16_static.lib libpng/lib/
cp libpngsrc/build/png*.h libpng/include/
cp libpngsrc/*.h libpng/include/

rm -rf libpngsrc
```
some headers might not be required, but well...

now they can be used in cmake to directly link and include:
```
set(PNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng/include")
set(PNG_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng/lib/libpng16_static.lib")

target_include_directories(${PROJECT_NAME} PRIVATE ${PNG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PNG_LIBRARY})
```
