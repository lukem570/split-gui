#ifndef SPLITGUI_EVENTS_HPP
#define SPLITGUI_EVENTS_HPP

#include "lib.hpp"

#include <string>
#include <unordered_map>

namespace SplitGui {

    class EventHandler;

    class Window;
    class Graphics;
    class Interface;
    class Scene;
    
    typedef void (*VariaticFunctionPointer)(...);

    enum class FunctionReturnType {
        eUnknown,
        eInt,
        eVoid,
        eFloat,
        eDouble,
        eString,
        eChar,
    };

    class EventAttachment {
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

            EventAttachment() : category(Category::eNone) {}
            EventAttachment(Category category, WindowType window)       : category(category), window(window) {}
            EventAttachment(Category category, GraphicsType graphics)   : category(category), graphics(graphics) {}
            EventAttachment(Category category, InterfaceType interface) : category(category), interface(interface) {}
            EventAttachment(Category category, SceneType scene)         : category(category), scene(scene) {}

            bool operator==(const EventAttachment& operand) const {
                switch (operand.category) {
                    case Category::eWindow: return operand.window == window; break;
                    case Category::eGraphics: return operand.graphics == graphics; break;
                    case Category::eInterface: return operand.interface == interface; break;
                    case Category::eScene: return operand.scene == scene; break;
                    default: return false; break;
                }
            }
    };
}

// hash implementation for attachments
namespace std {
    template <>
    struct hash<SplitGui::EventAttachment> {
        size_t operator()(const SplitGui::EventAttachment& e) const {
            return std::hash<int>{}((int)e.category * 20 + e.raw);
        }
    };
}

namespace SplitGui {

    template<typename T>
    constexpr FunctionReturnType enumerateReturnType() {
        if constexpr (std::is_same<T,int>::value) {
            return FunctionReturnType::eInt;
        } else if constexpr (std::is_same<T,void>::value) {
            return FunctionReturnType::eVoid;
        } else if constexpr (std::is_same<T,float>::value) {
            return FunctionReturnType::eFloat;
        } else if constexpr (std::is_same<T,double>::value) {
            return FunctionReturnType::eDouble;
        } else if constexpr (std::is_same<T,std::string>::value) {
            return FunctionReturnType::eString;
        } else if constexpr (std::is_same<T,char>::value) {
            return FunctionReturnType::eChar;
        } else {
            return FunctionReturnType::eUnknown;
        }
    }

    template <typename T>
    struct Callback {
        T (*function)(...);
    };

    struct CallbackData {
        unsigned int            paramCount;
        FunctionReturnType      returnType;
    };

    struct Context {
        Window*    pWindow;
        Graphics*  pGraphics;
        Interface* pInterface;
        Scene*     pScene;
    };

    class Event {
        public:
            friend class EventHandler;

            ~Event() {
                if (callback) {
                    free(callback);
                }
            }

            template <typename Return = void, typename... Args>
            Return call(Args... args) {
                if(enumerateReturnType<Return>() != callbackData.returnType) {
                    printf("WARN: return type provided doesn't match callback return type\n");
                    return Return();
                }
                
                if(sizeof...(args) != callbackData.paramCount) {
                    printf("WARN: invalid number of parameters in call\n");
                    return Return();
                }

                return ((Callback<Return>*)callback)->function(args...);
            }

        private:
            void*           callback = nullptr;
            CallbackData    callbackData;
            EventAttachment attachment;
    };

    class EventHandler {
        public:
            EventHandler();

            template<typename ReturnType, typename... Args>
            void bindFunction(ReturnType(*function)(Args...), std::string alias, EventAttachment attachment = EventAttachment()) {
                if (alias.size() <= 0) {
                    printf("WARN: Alias is invalid");
                    return;
                }

                CallbackData callbackData;
                callbackData.returnType = enumerateReturnType<ReturnType>();
                callbackData.paramCount = sizeof...(Args);

                Callback<ReturnType>* callback = (Callback<ReturnType>*)malloc(sizeof(Callback<ReturnType>));
                callback->function   = (ReturnType (*)(...))function;

                events.push_back({});

                events.back().callback     = callback;
                events.back().callbackData = callbackData;
                events.back().attachment   = attachment;

                stringMappings[alias] = &events.back();
                attachmentMappings[attachment] = &events.back();
            }

            Event* fetchEvent(std::string alias) {
                if (stringMappings.find(alias) == stringMappings.end()) {
                    return nullptr;
                }

                return stringMappings[alias];
            }

            Event* fetchEvent(EventAttachment attachment) {
                return attachmentMappings[attachment];
            }

            void attachWindow(Window* pWindow) {
                eventContext.pWindow = pWindow;
            }

            void attachGraphics(Graphics* pGraphics) {
                eventContext.pGraphics = pGraphics;
            }

            void attachInterface(Interface* pInterface) {
                eventContext.pInterface = pInterface;
            }

            void attachScene(Scene* pScene) {
                eventContext.pScene = pScene;
            }

            Context getContext() {
                return eventContext;
            }
        
        private:
            std::vector<Event>                          events;
            std::unordered_map<std::string, Event*>     stringMappings;
            std::unordered_map<EventAttachment, Event*> attachmentMappings;

            Context                                     eventContext;
    };
}

#endif