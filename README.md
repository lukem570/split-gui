# split gui
 A windowing library made in C++ with Vulkan and GLFW

---

## How to build 

#### fetch project dependencies

`python tools/moduleTools/modules.py`

#### build

`python tools/buildTools/build.py`

---

## build dependencies

* python
* ninja
* cmake
* git

## project dependencies

* GLFW
* glfwpp
* vulkan sdk 
* vulkan headers
* vulkan hpp
* volk

---

## Todo

* make xml parser

* refactor build tools
    * compile volk with modules
    * refactor project structure 
    * refactor error messages
    
* make the interface for the interface
* write documentation
* fix swapchain recreation
* draw text