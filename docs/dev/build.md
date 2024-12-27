# Build

This document covers instructions on how to build the project and fetch modules.

## build file locations

#### lib

`build/libsplitgui.so` or `build/libsplitgui.dll`

#### include

`include/splitgui`

## modules

#### windows

it is recomended to use the `tools/moduleTools/modules.py` script to download modules.

#### other

run the `tools/moduleTools/modules.py` script and install the vulkan sdk from the link.

`https://vulkan.lunarg.com/sdk/home#linux`

## Build project

#### Manual

configure

```
cmake -B build -G Ninja
```

build

```
cd build
ninja
```

#### With tools

run once:

```
tools/moduleTools/modules.py
```

run every test:

```
tools/testTools/test.py
```