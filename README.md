# split gui
 A windowing library made in C++ with Vulkan and GLFW

---

## How to build 

#### auto test

```
python tools/testTools/test.py
```

#### build

note: ninja is not required for builds

```
mkdir build
cd build
cmake -G "Ninja" .. 
ninja
```

---

## build dependencies

* python (optional)
* ninja  (optional)
* cmake
* git

## project dependencies

* GLFW
* glfwpp
* vulkan sdk 
* vulkan headers
* vulkan hpp
* volk
* msdfgen
* freetype

---

## Todo

* make xml parser

* refactor error messages

* write documentation

* draw text
* create >1 3d scene