# Split gui

Split gui is a GUI library that is entirely gpu focused therefore it has no dependencies 
for specific systems making it highly portable. Split gui is built on Vulkan and Glfw but is designed
to be expanded to other libraries and systems not implementing Vulkan or Glfw.

## Builds 

| Operating system |                            Build Status                            |      Notes      |
|------------------|--------------------------------------------------------------------|-----------------|
| Linux            | ![Passing](https://img.shields.io/badge/build-passing-brightgreen) |                 |
| Windows          | ![Unknown](https://img.shields.io/badge/build-unknown-yellow)      | not tested      |
| IOS              | ![Failing](https://img.shields.io/badge/build-failing-red)         | not implemented |
| Mac              | ![Failing](https://img.shields.io/badge/build-failing-red)         | not implemented |
| Android          | ![Failing](https://img.shields.io/badge/build-failing-red)         | not implemented |

## Documentation

All documentation is located in the [Docs](docs) folder and is seperated into user and developer documentation.

## Build

note: ninja is not required for builds

```
mkdir build
cd build
cmake -G "Ninja" .. 
ninja
```

## Build dependencies

* cmake
* git

## Project dependencies

* GLFW
* glfwpp
* vulkan sdk 
* vulkan headers
* vulkan hpp
* volk
* msdfgen
* freetype

## Todo

* make xml parser
* refactor error messages
* write documentation
* draw text
* create >1 3d scene
* interface updating
* constant sizes

## Updating buffers in the background idea

like an array list the buffer should double if required and the submit buffers function should track 
current buffer sizes and if it is almost full it sends a request to create a new buffer that is twice
as big or smaller maybe give creating a new buffer to another thread depending on the required size
