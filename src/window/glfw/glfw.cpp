#include "glfw.hpp"

#include "callbacks.cpp"
#include "cleanup.cpp"
#include "converters.cpp"
#include "function.cpp"
#include "getters.cpp"
#include "setup.cpp"

#ifdef SPLIT_GUI_USE_VULKAN
    #include "vulkan.cpp"
#endif