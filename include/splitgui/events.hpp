#ifndef SPLITGUI_EVENTS_HPP
#define SPLITGUI_EVENTS_HPP

#include "lib.hpp"

#include <splitgui/structs.hpp>
#include <string>
#include <stack>
#include <vector>

namespace SplitGui {

    class Window;
    class Graphics;
    class Interface;
    class Scene;

    enum class KeyState {
        ePress,
        eRelease,
        eHold,
    };

    enum class KeyCode {
        eNull         = '\0',
        eUnknown      = 255,
        
        eSpace        = ' ',
        eMinus        = '-',
        eAdd          = '+',

        ePeriod       = '.',
        eComma        = ',',

        eForwardSlash = '/',
        eBackslash    = '\\',

        eLeftBracket  = '[',
        eRightBracket = ']',

        eGraveAccent  = '`',
        eApostrophe   = '\'',

        eBackspace    = '\b',
        eTab          = '\t',
        eEnter        = '\n',
        eEscape       = 27,

        eDelete       = 127,
        eInsert       = 128,
        eHome         = 129,
        eEnd          = 130,
        ePause        = 131,
        ePrintScreen  = 132,

        ePageUp       = 133,
        ePageDown     = 134,

        eCapsLock     = 135,
        eScrollLock   = 136,
        eNumLock      = 137,

        eUp           = 138,
        eDown         = 139,
        eLeft         = 140,
        eRight        = 141,

        eF1           = 142,
        eF2           = 143,
        eF3           = 144,
        eF4           = 145,
        eF5           = 146,
        eF6           = 147,
        eF7           = 148,
        eF8           = 149,
        eF9           = 150,
        eF10          = 151,
        eF11          = 152,
        eF12          = 153,
        eF13          = 154,
        eF14          = 155,
        eF15          = 156,
        eF16          = 157,
        eF17          = 158,
        eF18          = 159,
        eF19          = 160,
        eF20          = 161,
        eF21          = 162,
        eF22          = 163,
        eF23          = 164,
        eF24          = 165,
        eF25          = 166,

        eKeyPad0      = 167,
        eKeyPad1      = 168,
        eKeyPad2      = 169,
        eKeyPad3      = 170,
        eKeyPad4      = 171,
        eKeyPad5      = 172,
        eKeyPad6      = 173,
        eKeyPad7      = 174,
        eKeyPad8      = 175,
        eKeyPad9      = 176,

        eKeyPadPoint  = 177,
        eKeyPadDivide = 178,
        eKeyPadStar   = 179,
        eKeyPadMinus  = 180, 
        eKeyPadAdd    = 181,
        eKeyPadEnter  = 182,
        eKeyPadEqual  = 183,
        
        eLeftShift    = 184,
        eLeftControl  = 185,
        eLeftAlt      = 186,
        eLeftSuper    = 187,
        
        eRightShift   = 188, 
        eRightControl = 189,
        eRightAlt     = 190,
        eRightSuper   = 191,

        eMenu         = 192,

        eZero         = '0',
        eOne          = '1',
        eTwo          = '2',
        eThree        = '3',
        eFour         = '4',
        eFive         = '5',
        eSix          = '6',
        eSeven        = '7',
        eEight        = '8',
        eNine         = '9',
        eSemicolon    = ';',
        eEqual        = '=',

        eA            = 'a',
        eB            = 'b',
        eC            = 'c',
        eD            = 'd',
        eE            = 'e',
        eF            = 'f',
        eG            = 'g',
        eH            = 'h',
        eI            = 'i',
        eJ            = 'j',
        eK            = 'k',
        eL            = 'l',
        eM            = 'm',
        eN            = 'n',
        eO            = 'o',
        eP            = 'p',
        eQ            = 'q',
        eR            = 'r',
        eS            = 's',
        eT            = 't',
        eU            = 'u',
        eV            = 'v',
        eW            = 'w',
        eX            = 'x',
        eY            = 'y',
        eZ            = 'z',
    };

    enum class MouseState {
        ePress,
        eRelease,
    };

    enum class MouseCode {
        eUnknown = 255,

        eOne,   // left
        eTwo,   // right
        eThree, // middle

        eFour,
        eFive,
        eSix,
        eSeven,
        eEight,
    };

    union WindowEventData {

        struct {
            IVec2 size;
        } resize;

        struct {
            KeyCode  keyCode;
            KeyState keyState;
        } keypress;

        struct {
            MouseCode mouseCode;
            MouseState mouseState;
        } mouseButton;

        struct {
            int xPos;
            int yPos;
        } mouseMove;
    };

    struct InterfaceEventData {

        enum class Type {
            eFunctionCall,
        };

        struct FunctionCall{
            std::string alias;
            std::vector<UnitExpressionValue> params;
            UnitExpressionValue* returnValue;
        };

        union {
            FunctionCall functionCall;
        };
        Type type;

        InterfaceEventData operator=(const InterfaceEventData& data);
        InterfaceEventData(const InterfaceEventData& data);
        InterfaceEventData() : functionCall{} {}
        ~InterfaceEventData();
    };

    struct EventData {
        enum class Type {
            eWindow,
            eInterface,
        };

        union {
            WindowEventData window;
            InterfaceEventData interface;
        };
        Type type;

        EventData operator=(const EventData& data);
        EventData(const EventData& data);
        EventData() : window{} {}
        ~EventData();
    };

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
                eFunctionCall
            };

            enum class SceneType {

            };

            union {
                WindowType    window;
                GraphicsType  graphics;
                Event::InterfaceType interface_;
                SceneType     scene;
                int raw;
            };
            Category category;
            EventData data;

            Event() : category(Category::eNone) {}
            Event(Category category, WindowType window)       : category(category), window(window) {}
            Event(Category category, GraphicsType graphics)   : category(category), graphics(graphics) {}
            Event(Category category, InterfaceType interface_) : category(category), interface_(interface_) {}
            Event(Category category, SceneType scene)         : category(category), scene(scene) {}

            bool operator==(const Event& operand) const {
                switch (operand.category) {
                    case Category::eWindow: return operand.window == window; break;
                    case Category::eGraphics: return operand.graphics == graphics; break;
                    case Category::eInterface: return operand.interface_ == interface_; break;
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

    class SPLITGUI_EXPORT EventHandler {
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