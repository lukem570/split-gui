#ifndef SPLITGUI_EVENTS_HPP
#define SPLITGUI_EVENTS_HPP

#include "lib.hpp"

#include <string>
#include <unordered_map>

namespace SplitGui {

    class EventHandler;
    
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
            void* callback = nullptr;
            CallbackData callbackData;
    };

    class EventHandler {
        public:

            template<typename ReturnType, typename... Args>
            void bindFunction(ReturnType(*function)(Args...), std::string alias) {
                if (alias.size() <= 0) {
                    printf("WARN: Alias is invalid");
                    return;
                }

                CallbackData callbackData;
                callbackData.returnType = enumerateReturnType<ReturnType>();
                callbackData.paramCount = sizeof...(Args);

                Callback<ReturnType>* callback = (Callback<ReturnType>*)malloc(sizeof(Callback<ReturnType>));
                callback->function   = (ReturnType (*)(...))function;

                events[alias].callback     = callback;
                events[alias].callbackData = callbackData;
            }

            Event* fetchEvent(std::string alias) {
                return &events[alias];
            }
        
        private:
            std::unordered_map<std::string, Event> events;
    };
}

#endif