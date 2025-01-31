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

``` bash
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

## How to use

#### Basic xml handler

main.cpp
``` c++
#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

#include <fstream>
#include <sstream>
#include <string>

int main(void) {

    std::ifstream indexFile("index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("xml test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(true));
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    SplitGui::RectObj viewport;
    viewport.size = window.getSize();
    viewport.x    = 0;
    viewport.y    = 0;
    
    SplitGui::Interface interface;
    TRYRC(parseRes, interface.parseXml(page));
    interface.submitGraphics(graphics);
    interface.setViewport(viewport);
    interface.attachEventHandler(eventHandler);

    eventHandler.instanceBuiltinEvents();
    interface.instance();
    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }
    
    return 0;
}
```
index.xml
``` xml
<meta version="0.1.0"/>


<split direction="vertical" position="20px + 50%"> 
    <rect color="ivec3(255, 100, 100)"/>
    <rect color="ivec3(100, 255, 100)"/>
</split>
```

## Todo

### Features

* create >1 3d scene
* write documentation
* add scene tree to xml
* finish xml parser 
* finish unit expression parser
* make config for builds

### Optimizations

* pass in matrix to vertex shader for scenes
* staging buffer caching
* make staging buffer allocate extra space for update events
* multithreading?
* fix scenes buffers recreating on updateposition and updaterotation
    * use a custom copy region for specific indices

### Refactoring

* refactor xml parser
* convension check
* general refactor
* refactor text code

### Bugs

* rot x not working in scenes  
    * check memory allignment and buffer creation
* slowness
    * performance testing and checking for bottlenecks