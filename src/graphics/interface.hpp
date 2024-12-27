#include <vulkan/vulkan.hpp>
#include <glfwpp/glfwpp.h>
#include <vector>
#include <tuple>
#include <string>

class GraphicsLibInterface {
    public:
        virtual void instance() {}
        virtual void submitWindow(glfw::Window* window) {}

    protected:
        glfw::Window* pWindow;
};