#include <splitgui/window.hpp>

namespace SplitGui {
    class GlfwInterface : WindowLibInterface {
        public:
            GlfwInterface() {
                glfwInit();
            }

            ~GlfwInterface() {
                if (window.handle) {
                    delete window.handle;
                }

                glfwTerminate();
            }

            virtual void createWindow(const char* title) {
                glfw::WindowHints hints;
                hints.clientApi = (glfw::ClientApi) 0L;
                hints.resizable = false;
                hints.visible   = true;
                hints.decorated = true;

                hints.apply();
                
                window.handle = new glfw::Window(640, 480, title);

                glfw::makeContextCurrent(*window.handle);

                window.handle->swapBuffers();


            }
            
            virtual RawWindow* getWindowData() {
                return &window;
            }

        private:
            RawWindow window;

    };
}