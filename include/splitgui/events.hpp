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

    template<typename ReturnType, typename... Args>
    constexpr FunctionReturnType getReturnTypeOfFunctionPointer(ReturnType(*func)(Args...)) {
        return enumerateReturnType<ReturnType>();
    }

    template <typename Func>
    struct FunctionTraits;

    template <typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType(*)(Args...)> {
        static constexpr size_t argCount = sizeof...(Args);
    };
    
    template<typename R, typename... ARGS>
    static R return_type(R (*)(ARGS...));

    template<auto FUNCTION_POINTER>
    using ReturnType = decltype(return_type(FUNCTION_POINTER));

    struct Callback {
        VariaticFunctionPointer function;
        unsigned int            paramCount;
        FunctionReturnType      returnType;
    };

    class Event {
        public:
            friend class EventHandler;

        private:
            Callback callback;
    };

    class EventHandler {
        public:

            template <typename T>
            void bindFunction(T function, std::string alias) {
                if (alias.size() <= 0) {
                    printf("WARN: Alias is invalid");
                    return;
                }

                Callback callback;
                callback.paramCount =        FunctionTraits<decltype(function)>::argCount;
                callback.returnType = getReturnTypeOfFunctionPointer(function);
                callback.function   =       (VariaticFunctionPointer)function;

                events[alias].callback = callback;
            }

            Event* fetchEvent(std::string alias) {

            }
        
        private:
            std::unordered_map<std::string, Event> events;
    };
}

#endif