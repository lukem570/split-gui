#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
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

                window.handle->swapBuffers();


            }
            
            RawWindow* getWindowData() override {
                return &window;
            }

            IVec2 getSize() {
                IVec2 size;
                std::tuple<int, int> sizeTuple = window.handle->getSize();
                size.x = std::get<0>(sizeTuple);
                size.y = std::get<1>(sizeTuple);
                return size;
            }

        private:
            RawWindow window;

    };
}