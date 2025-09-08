#!/bin/sh

rm -rf build/
rm -rf dist/
rm -rf packages/
rm -rf _CPack_Packages/

#conan install . --output-folder=build --build=missing --profile=default
#cmake . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/suso/Qt/6.7.3/gcc_64 -B build/
#cmake --build build/ --parallel $(nproc --all)

conan install . --build=missing -s build_type=Release
cmake --preset conan-release -DCMAKE_PREFIX_PATH=/home/suso/Qt/6.7.3/gcc_64
cmake --build --preset conan-release --parallel $(nproc --all)
cp -r data build/Release

conan install . --build=missing -s build_type=Debug
cmake --preset conan-debug -DCMAKE_PREFIX_PATH=/home/suso/Qt/6.7.3/gcc_64
cmake --build --preset conan-debug --parallel $(nproc --all)
cp -r data build/Debug

cmake --install build/Release --prefix $(pwd)/dist
#cpack --config build/Release/CPackConfig.cmake

