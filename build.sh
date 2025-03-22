#!/bin/sh

#rm -rf build/
conan install . --output-folder=build --build=missing
cmake . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build/
cmake --build build/ --parallel $(nproc --all)
