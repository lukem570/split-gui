# Getting started

This document outlines the step by step process on how to use split-gui at the basic level.

## Result values

split-gui uses result values. Here are some basic uses

### Header

``` c++
#include <splitgui/result.hpp>
```

#### Result Enums

``` c++
// Result enum
SplitGui::Result result = foo();

if (result != SplitGui::Result::eSuccess) {
    // Handle error
}
```

#### Result Values

``` c++
SplitGui::ResultValue<int> valueRes = bar();

if (valueRes.result != SplitGui::Result::eSuccess) {
    // Handle error
}

// Use value
```

#### Result Macros
``` c++
// returns a result if resultvalue != eSuccess and discards the value
TRY(type, name, resultValue);

// returns a result if resultvalue != eSuccess
TRYD(resultValue);

// returns a result casted to an int if resultvalue != eSuccess and discards the value
TRYC(type, name, resultValue);

// returns a result if result != eSuccess
TRYR(name, result)

// returns a result casted to an int if result != eSuccess
TRYRC(name, result)
```

## Event Handler 

The event handler object is responsible for distributing events and calls throughout the application.

### Header

``` c++
#include <splitgui/events.hpp>
```

## Window

The window object is responsible for handing data from the operating system.

### Header

``` c++
#include <splitgui/window.hpp>
```

#### Create Event Handler

``` c++
SplitGui::EventHandler eventHandler;
```

#### Open a Window

``` c++
SplitGui::Window window;

// Use GLFW as the window handler
TRYRC(glfwRes, window.instanceGlfw());

// Open a window with the title 'Hello, Window'
TRYRC(windowRes, window.createWindow("Hello, Window"));

// Attach event handler
window.attachEventHandler(eventHandler);

// ...

while (!window.shouldClose()) {
    window.update();
}
```

## Graphics

The graphics object is responsible for managing rendering of interfaces and scenes.

### Header

``` c++
#include <splitgui/graphics.hpp>
```

#### Create the Graphics Object

``` c++

SplitGui::VulkanFlags vulkanFlags;

// enable validation layers
vulkanFlags.enableValidationLayers = true;

SplitGui::Graphics graphics;

// Use Vulkan as the graphics library
TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));

// Submit the window object
graphics.submitWindow(window);

// Attach the event hander
graphics.attachEventHandler(eventHandler);

// ...

while (!window.shouldClose()) {
    graphics.drawFrame();
    window.update();
}
```

## Interface

The interface object is responsible for managing 2d UI and parsing data (e.g XML).

### Header

``` c++
#include <splitgui/interface.hpp>
```

#### Creating An Interface With XML

``` xml
<meta version="0.1.0"/>

<split direction="horizontal" position="50%"> 
    <split direction="vertical" position="50%"> 
        <rect color="ivec3(255u, 100u, 100u)"/>
        <rect color="ivec3(100u, 100u, 255u)"/>
    </split>
    <rect color="ivec3(100u, 255u, 100u)"/>
</split>
```

``` c++

std::string index = // Load xml as a string here

// Create the viewport for the interface
SplitGui::RectObj viewport;
viewport.size   = window.getSize();
viewport.x      = 0;
viewport.y      = 0;


SplitGui::Interface ui;

// Parse XML
TRYRC(interfaceRes, ui.parseXml(index));

// Submit the graphics object
ui.submitGraphics(graphics);

// Set the viewport
ui.setViewport(viewport);

// Attach the event handler 
ui.attachEventHandler(eventHandler);

// ...

// Instance interface
ui.instance();

// Submit Buffers
graphics.submitBuffers();
```