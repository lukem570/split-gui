#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/lib.h>
#include <tuple>

namespace SplitGui {
    class GlfwInterface : WindowLibInterface {
        public:
            GlfwInterface() {
                glfwInit();
                window.type = FormatType::eGlfw;
            }

            ~GlfwInterface() {
                if (window.handle) {
                    delete window.handle;
                }

                glfwTerminate();
            }

            void createWindow(const char* title) override {
                glfw::WindowHints hints;
                hints.clientApi = (glfw::ClientApi) 0L;
                hints.resizable = true;
                hints.visible   = true;
                hints.decorated = true;

                hints.apply();
                
                window.handle = new glfw::Window(640, 480, title);

                glfw::makeContextCurrent(*window.handle);

                window.handle->framebufferSizeEvent.setCallback(resize_callback);
                

                window.handle->swapBuffers();
            }
            
            RawWindow* getWindowData() override {
                return &window;
            }

            IVec2 getSize() override {
                IVec2 size;
                std::tuple<int, int> sizeTuple = window.handle->getSize();
                size.x = std::get<0>(sizeTuple);
                size.y = std::get<1>(sizeTuple);
                return size;
            }

            bool shouldClose() override {
                return window.handle->shouldClose();
            }

            void update() override {
                window.handle->swapBuffers();
                glfw::pollEvents();
            }

#ifdef SPLIT_GUI_USE_VULKAN
            
            vk::SurfaceKHR createSurface(vk::Instance instance) override {
                #if defined(__linux__) 

                    #if defined(VK_USE_PLATFORM_WAYLAND_KHR)
                        vk::WaylandSurfaceCreateInfoKHR createInfo;
                        createInfo.display = glfwGetWaylandDisplay();
                        createInfo.surface = glfwGetWaylandWindow(window);

                        if (createInfo.display == nullptr || createInfo.surface == nullptr) {
                            printf("Error getting window handles for Wayland\n");
                            throw;
                        }

                        return instance.createWaylandSurfaceKHR(createInfo);
                    #elif defined(VK_USE_PLATFORM_XCB_KHR)
                        vk::XcbSurfaceCreateInfoKHR createInfo;
                        createInfo.connection = glfwGetX11Display();
                        createInfo.window     = glfwGetX11Window(window);

                        if (createInfo.connection == nullptr || createInfo.window == 0) {
                            printf("Error getting window handles for XCB\n");
                            throw;
                        }

                        return instance.createXcbSurfaceKHR(createInfo);
                    #elif defined(VK_USE_PLATFORM_XLIB_KHR)
                        vk::XlibSurfaceCreateInfoKHR createInfo;
                        createInfo.dpy    = glfwGetX11Display();
                        createInfo.window = glfwGetX11Window(*window.handle);

                        if (createInfo.dpy == None || createInfo.window == None) {
                            printf("Error getting window handles for xlib\n");
                            throw;
                        }
                        
                        return instance.createXlibSurfaceKHR(createInfo);
                    #endif

                #elif defined(_WIN32)
                    vk::Win32SurfaceCreateInfoKHR createInfo;
                    createInfo.hinstance = GetModuleHandle(nullptr);
                    createInfo.hwnd      = glfwGetWin32Window(window);

                    if (createInfo.hwnd == nullptr) {
                        printf("Error getting window handle for win32\n");
                        throw;
                    }

                    return instance.createWin32SurfaceKHR(createInfo);     
                #endif
            }

#endif 

        private:
            RawWindow window;

            void submitGraphics(Graphics* pGraphics) override {

                printf("graphics submitted\n");

                window.pGraphics = pGraphics;

                window.handle->setUserPointer(pGraphics);
            }

            static void resize_callback(glfw::Window& window, int width, int height) {
                
                ((Graphics*)window.getUserPointer())->resizeEvent();
                
                printf("Window resized to %d x %d\n", width, height);
            }

    };
}