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
        Window*    pWindow    = nullptr;
        Graphics*  pGraphics  = nullptr;
        Interface* pInterface = nullptr;
        Scene*     pScene     = nullptr;
    };

    class Event {
        public:
            friend class EventHandler;

            ~Event() {
                for (int i = 0; i < callbacks.size(); i++) {
                    free(callbacks[i]);
                }
            }

            template <typename Return, typename... Args>
            std::vector<Return> callRet(Args... args) {
                std::vector<Return> ret;

                for (int i = 0; i < callbacks.size(); i++) {
                    if(enumerateReturnType<Return>() != callbackDatas[i].returnType) {
                        printf("WARN: return type provided doesn't match callback return type\n");
                        return {};
                    }
                    
                    if(sizeof...(args) != callbackDatas[i].paramCount) {
                        printf("WARN: invalid number of parameters in call\n");
                        return {};
                    }

                    ret.push_back(((Callback<Return>*)callbacks[i])->function(args...));
                }
                
                return ret;
            }

            template <typename... Args>
            void call(Args... args) {
                for (int i = 0; i < callbacks.size(); i++) {
                    if(enumerateReturnType<void>() != callbackDatas[i].returnType) {
                        printf("WARN: return type provided doesn't match callback return type\n");
                        return;
                    }
                    
                    if(sizeof...(args) != callbackDatas[i].paramCount) {
                        printf("WARN: invalid number of parameters in call\n");
                        return;
                    }

                    ((Callback<void>*)callbacks[i])->function(args...);
                }
            }

        private:
            std::vector<void*>        callbacks;
            std::vector<CallbackData> callbackDatas;
    };

    class EventHandler {
        public:
            void instanceBuiltinEvents();

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
                callback->function             = (ReturnType (*)(...))function;

                int event;

                if (stringMappings.find(alias) != stringMappings.end()) {
                    event = stringMappings[alias];
                } else if (attachmentMappings.find(attachment) != attachmentMappings.end()) {
                    if (attachment.category != EventAttachment::Category::eNone) {
                        event = attachmentMappings[attachment];
                    }
                } else {
                    event = events.size();
                    events.push_back({});
                }

                events[event].callbacks.push_back(callback);
                events[event].callbackDatas.push_back(callbackData);

                stringMappings[alias] = event;
                attachmentMappings[attachment] = event;
            }

            Event* fetchEvent(std::string alias) {
                if (stringMappings.find(alias) == stringMappings.end()) {
                    return nullptr;
                }

                return &events[stringMappings[alias]];
            }

            Event* fetchEvent(EventAttachment attachment) {
                if (attachmentMappings.find(attachment) == attachmentMappings.end()) {
                    return nullptr;
                }

                return &events[attachmentMappings[attachment]];
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
            std::vector<Event>                       events;
            std::unordered_map<std::string, int>     stringMappings;
            std::unordered_map<EventAttachment, int> attachmentMappings;

            Context                                     eventContext;
    };
}

#endif