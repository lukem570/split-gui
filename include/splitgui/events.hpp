#ifndef SPLITGUI_EVENTS_HPP
#define SPLITGUI_EVENTS_HPP

#include "lib.hpp"

#include <string>
#include <stack>

namespace SplitGui {

    class Window;
    class Graphics;
    class Interface;
    class Scene;

    class Event {
        public:
            enum class Category{
                eNone,
                eWindow,
                eGraphics,
                eInterface,
                eScene,
            };

            enum class WindowType {
                eResize,
                eKeypress,
                eMouseButton,
                eMouseMove,
            };

            enum class GraphicsType {

            };

            enum class InterfaceType {
                eBinding
            };

            enum class SceneType {

            };

            union {
                WindowType    window;
                GraphicsType  graphics;
                InterfaceType interface;
                SceneType     scene;
                int raw;
            };
            Category category;

            Event() : category(Category::eNone) {}
            Event(Category category, WindowType window)       : category(category), window(window) {}
            Event(Category category, GraphicsType graphics)   : category(category), graphics(graphics) {}
            Event(Category category, InterfaceType interface) : category(category), interface(interface) {}
            Event(Category category, SceneType scene)         : category(category), scene(scene) {}

            bool operator==(const Event& operand) const {
                switch (operand.category) {
                    case Category::eWindow: return operand.window == window; break;
                    case Category::eGraphics: return operand.graphics == graphics; break;
                    case Category::eInterface: return operand.interface == interface; break;
                    case Category::eScene: return operand.scene == scene; break;
                    default: return false; break;
                }
            }
    };

    struct Context {
        Window*    pWindow    = nullptr;
        Graphics*  pGraphics  = nullptr;
        Interface* pInterface = nullptr;
        Scene*     pScene     = nullptr;
    };

    class EventHandler {
        public:

            int   popEvent();
            void  pushEvent(Event event);
            Event getEvent();

            void attachWindow(Window* pWindow);
            void attachGraphics(Graphics* pGraphics);
            void attachInterface(Interface* pInterface);
            void attachScene(Scene* pScene);

        private:
            std::stack<Event> events;
            Event             eventBuffer;
            Context           eventContext;

            void callBuiltinEvent(Event event);

    };
}

#endif