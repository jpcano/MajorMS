#!/bin/sh

rm -rf build/

#conan install . --output-folder=build --build=missing
#cmake . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build/
#cmake --build build/ --parallel $(nproc --all)

conan install . --output-folder=build
#conan install . --output-folder=build --settings=build_type=Debug
cmake --preset conan-release
#cmake --preset conan-debug
cmake --build --preset conan-release --parallel $(nproc --all)

