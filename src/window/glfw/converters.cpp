#include "glfw.hpp"

namespace SplitGui {
    KeyCode GlfwInterface::convertGlfwKeyCode(glfw::KeyCode code) {
        switch (code) {
            case glfw::KeyCode::Unknown:        return KeyCode::eUnknown;           break;
            case glfw::KeyCode::Space:          return KeyCode::eSpace;             break;
            case glfw::KeyCode::Apostrophe:     return KeyCode::eApostrophe;        break;
            case glfw::KeyCode::Comma:          return KeyCode::eComma;             break;
            case glfw::KeyCode::Minus:          return KeyCode::eMinus;             break;
            case glfw::KeyCode::Period:         return KeyCode::ePeriod;            break;
            case glfw::KeyCode::Slash:          return KeyCode::eForwardSlash;      break;
            case glfw::KeyCode::Backslash:      return KeyCode::eBackslash;         break;
            case glfw::KeyCode::Zero:           return KeyCode::eZero;              break;
            case glfw::KeyCode::One:            return KeyCode::eOne;               break;
            case glfw::KeyCode::Two:            return KeyCode::eTwo;               break;
            case glfw::KeyCode::Three:          return KeyCode::eThree;             break;
            case glfw::KeyCode::Four:           return KeyCode::eFour;              break;
            case glfw::KeyCode::Five:           return KeyCode::eFive;              break;
            case glfw::KeyCode::Six:            return KeyCode::eSix;               break;
            case glfw::KeyCode::Seven:          return KeyCode::eSeven;             break;
            case glfw::KeyCode::Eight:          return KeyCode::eEight;             break;
            case glfw::KeyCode::Nine:           return KeyCode::eNine;              break;
            case glfw::KeyCode::Semicolon:      return KeyCode::eSemicolon;         break;
            case glfw::KeyCode::Equal:          return KeyCode::eEqual;             break;
            case glfw::KeyCode::A:              return KeyCode::eA;                 break;
            case glfw::KeyCode::B:              return KeyCode::eB;                 break;
            case glfw::KeyCode::C:              return KeyCode::eC;                 break;
            case glfw::KeyCode::D:              return KeyCode::eD;                 break;
            case glfw::KeyCode::E:              return KeyCode::eE;                 break;
            case glfw::KeyCode::F:              return KeyCode::eF;                 break;
            case glfw::KeyCode::G:              return KeyCode::eG;                 break;
            case glfw::KeyCode::H:              return KeyCode::eH;                 break;
            case glfw::KeyCode::I:              return KeyCode::eI;                 break;
            case glfw::KeyCode::J:              return KeyCode::eJ;                 break;
            case glfw::KeyCode::K:              return KeyCode::eK;                 break;
            case glfw::KeyCode::L:              return KeyCode::eL;                 break;
            case glfw::KeyCode::M:              return KeyCode::eM;                 break;
            case glfw::KeyCode::N:              return KeyCode::eN;                 break;
            case glfw::KeyCode::O:              return KeyCode::eO;                 break;
            case glfw::KeyCode::P:              return KeyCode::eP;                 break;
            case glfw::KeyCode::Q:              return KeyCode::eQ;                 break;
            case glfw::KeyCode::R:              return KeyCode::eR;                 break;
            case glfw::KeyCode::S:              return KeyCode::eS;                 break;
            case glfw::KeyCode::T:              return KeyCode::eT;                 break;
            case glfw::KeyCode::U:              return KeyCode::eU;                 break;
            case glfw::KeyCode::V:              return KeyCode::eV;                 break;
            case glfw::KeyCode::W:              return KeyCode::eW;                 break;
            case glfw::KeyCode::X:              return KeyCode::eX;                 break;
            case glfw::KeyCode::Y:              return KeyCode::eY;                 break;
            case glfw::KeyCode::Z:              return KeyCode::eZ;                 break;
            case glfw::KeyCode::LeftBracket:    return KeyCode::eLeftBracket;       break;
            case glfw::KeyCode::RightBracket:   return KeyCode::eRightBracket;      break;
            case glfw::KeyCode::GraveAccent:    return KeyCode::eGraveAccent;       break;
            case glfw::KeyCode::Escape:         return KeyCode::eEscape;            break;
            case glfw::KeyCode::Enter:          return KeyCode::eEnter;             break;
            case glfw::KeyCode::Tab:            return KeyCode::eTab;               break;
            case glfw::KeyCode::Backspace:      return KeyCode::eBackspace;         break;
            case glfw::KeyCode::Insert:         return KeyCode::eInsert;            break;
            case glfw::KeyCode::Delete:         return KeyCode::eDelete;            break;
            case glfw::KeyCode::Right:          return KeyCode::eRight;             break;
            case glfw::KeyCode::Left:           return KeyCode::eLeft;              break;
            case glfw::KeyCode::Down:           return KeyCode::eDown;              break;
            case glfw::KeyCode::Up:             return KeyCode::eUp;                break;
            case glfw::KeyCode::PageUp:         return KeyCode::ePageUp;            break;
            case glfw::KeyCode::PageDown:       return KeyCode::ePageDown;          break;
            case glfw::KeyCode::Home:           return KeyCode::eHome;              break;
            case glfw::KeyCode::End:            return KeyCode::eEnd;               break;
            case glfw::KeyCode::CapsLock:       return KeyCode::eCapsLock;          break;
            case glfw::KeyCode::ScrollLock:     return KeyCode::eScrollLock;        break;
            case glfw::KeyCode::NumLock:        return KeyCode::eNumLock;           break;
            case glfw::KeyCode::PrintScreen:    return KeyCode::ePrintScreen;       break;
            case glfw::KeyCode::Pause:          return KeyCode::ePause;             break;
            case glfw::KeyCode::F1:             return KeyCode::eF1;                break;
            case glfw::KeyCode::F2:             return KeyCode::eF2;                break;
            case glfw::KeyCode::F3:             return KeyCode::eF3;                break;
            case glfw::KeyCode::F4:             return KeyCode::eF4;                break;
            case glfw::KeyCode::F5:             return KeyCode::eF5;                break;
            case glfw::KeyCode::F6:             return KeyCode::eF6;                break;
            case glfw::KeyCode::F7:             return KeyCode::eF7;                break;
            case glfw::KeyCode::F8:             return KeyCode::eF8;                break;
            case glfw::KeyCode::F9:             return KeyCode::eF9;                break;
            case glfw::KeyCode::F10:            return KeyCode::eF10;               break;
            case glfw::KeyCode::F11:            return KeyCode::eF11;               break;
            case glfw::KeyCode::F12:            return KeyCode::eF12;               break;
            case glfw::KeyCode::F13:            return KeyCode::eF13;               break;
            case glfw::KeyCode::F14:            return KeyCode::eF14;               break;
            case glfw::KeyCode::F15:            return KeyCode::eF15;               break;
            case glfw::KeyCode::F16:            return KeyCode::eF16;               break;
            case glfw::KeyCode::F17:            return KeyCode::eF17;               break;
            case glfw::KeyCode::F18:            return KeyCode::eF18;               break;
            case glfw::KeyCode::F19:            return KeyCode::eF19;               break;
            case glfw::KeyCode::F20:            return KeyCode::eF20;               break;
            case glfw::KeyCode::F21:            return KeyCode::eF21;               break;
            case glfw::KeyCode::F22:            return KeyCode::eF22;               break;
            case glfw::KeyCode::F23:            return KeyCode::eF23;               break;
            case glfw::KeyCode::F24:            return KeyCode::eF24;               break;
            case glfw::KeyCode::F25:            return KeyCode::eF25;               break;
            case glfw::KeyCode::KeyPad0:        return KeyCode::eKeyPad0;           break;
            case glfw::KeyCode::KeyPad1:        return KeyCode::eKeyPad1;           break;
            case glfw::KeyCode::KeyPad2:        return KeyCode::eKeyPad2;           break;
            case glfw::KeyCode::KeyPad3:        return KeyCode::eKeyPad3;           break;
            case glfw::KeyCode::KeyPad4:        return KeyCode::eKeyPad4;           break;
            case glfw::KeyCode::KeyPad5:        return KeyCode::eKeyPad5;           break;
            case glfw::KeyCode::KeyPad6:        return KeyCode::eKeyPad6;           break;
            case glfw::KeyCode::KeyPad7:        return KeyCode::eKeyPad7;           break;
            case glfw::KeyCode::KeyPad8:        return KeyCode::eKeyPad8;           break;
            case glfw::KeyCode::KeyPad9:        return KeyCode::eKeyPad9;           break;
            case glfw::KeyCode::KeyPadDecimal:  return KeyCode::eKeyPadPoint;       break;
            case glfw::KeyCode::KeyPadDivide:   return KeyCode::eKeyPadDivide;      break;
            case glfw::KeyCode::KeyPadMultiply: return KeyCode::eKeyPadStar;        break;
            case glfw::KeyCode::KeyPadSubtract: return KeyCode::eKeyPadMinus;       break;
            case glfw::KeyCode::KeyPadAdd:      return KeyCode::eKeyPadAdd;         break;
            case glfw::KeyCode::KeyPadEnter:    return KeyCode::eKeyPadEnter;       break;
            case glfw::KeyCode::KeyPadEqual:    return KeyCode::eKeyPadEqual;       break;
            case glfw::KeyCode::LeftShift:      return KeyCode::eLeftShift;         break;
            case glfw::KeyCode::LeftControl:    return KeyCode::eLeftControl;       break;
            case glfw::KeyCode::LeftAlt:        return KeyCode::eLeftAlt;           break;
            case glfw::KeyCode::LeftSuper:      return KeyCode::eLeftSuper;         break;
            case glfw::KeyCode::RightShift:     return KeyCode::eRightShift;        break;
            case glfw::KeyCode::RightControl:   return KeyCode::eRightControl;      break;
            case glfw::KeyCode::RightAlt:       return KeyCode::eRightAlt;          break;
            case glfw::KeyCode::RightSuper:     return KeyCode::eRightSuper;        break;
            case glfw::KeyCode::Menu:           return KeyCode::eMenu;              break;
            default:                            return KeyCode::eNull;              break;
        }
    }

    inline KeyState GlfwInterface::convertGlfwKeyState(glfw::KeyState state) {
        switch (state) {
            case glfw::KeyState::Press:   return KeyState::ePress;   break;
            case glfw::KeyState::Release: return KeyState::eRelease; break;
            case glfw::KeyState::Repeat:  return KeyState::eHold;    break;
        }

        return KeyState::eHold; // TODO: fix
    }

    inline MouseCode GlfwInterface::convertGlfwMouseButton(glfw::MouseButton code) {
        switch (code) {
            case glfw::MouseButton::One:   return MouseCode::eOne;   break;
            case glfw::MouseButton::Two:   return MouseCode::eTwo;   break;
            case glfw::MouseButton::Three: return MouseCode::eThree; break;
            case glfw::MouseButton::Four:  return MouseCode::eFour;  break;
            case glfw::MouseButton::Five:  return MouseCode::eFive;  break;
            case glfw::MouseButton::Six:   return MouseCode::eSix;   break;
            case glfw::MouseButton::Seven: return MouseCode::eSeven; break;
            case glfw::MouseButton::Eight: return MouseCode::eEight; break;
        }

        return MouseCode::eUnknown;
    }

    inline MouseState GlfwInterface::convertGlfwMouseState(glfw::MouseButtonState state) {
        switch (state) {
            case glfw::MouseButtonState::Press:   return MouseState::ePress;   break;
            case glfw::MouseButtonState::Release: return MouseState::eRelease; break;
        }

        return MouseState::ePress; // TODO: fix
    }

}