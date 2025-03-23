#!/bin/sh

rm -rf build/
rm -rf dist/
rm -rf pack/

#conan install . --output-folder=build --build=missing
#cmake . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build/
#cmake --build build/ --parallel $(nproc --all)

conan install . --output-folder=build
cmake --preset conan-release
cmake --build --preset conan-release --parallel $(nproc --all)

cmake --install build --prefix dist
mkdir pack
#(cd pack && cpack --config ../build/CPackConfig.cmake && cpack --config ../build/CPackSourceConfig.cmake)
(cd pack && cpack --config ../build/CPackConfig.cmake)

