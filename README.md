# MajorMS

Major Mnemonic System (MajorMS), is a tool that converts numbers into consonants, then into words by adding vowels.

It is used to help in memorizing numbers following  major system (also called the phonetic number system, phonetic mnemonic system, or Hérigone's mnemonic system). The system works on the principle that images can be remembered more easily than numbers.

## Build instructions

1. Use conan to download the depencencies and generate the files that CMake needs:

```shell
conan install . --output-folder=build --build=missing
```

2. Use CMake to build the project:

```shell
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
