#!/bin/sh

rm -rf build/
rm -rf dist/
rm -rf pack/

#conan install . --output-folder=build --build=missing
#cmake . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build/
#cmake --build build/ --parallel $(nproc --all)

conan install . --output-folder=build
cmake --preset conan-release -DCMAKE_PREFIX_PATH=/home/suso/Qt/6.7.3/gcc_64
cmake --build --preset conan-release --parallel $(nproc --all)

cmake --install build --prefix $(pwd)/dist
mkdir pack
(cd pack && cpack --config ../build/CPackConfig.cmake)

