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

