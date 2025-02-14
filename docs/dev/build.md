# Build

This document covers instructions on how to build the project and fetch modules.

## build file locations

#### lib

`build/libsplitgui.so` or `build/libsplitgui.dll`

#### include

`include/splitgui`

#### shaders

`include/shaders`

## Build project

#### Manual

configure

```
cmake -B build -G "Ninja"
```

build

```
cd build
ninja
```

#### Test

```
tools/testTools/test.py
```