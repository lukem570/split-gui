#include <splitgui/interface.hpp>

#include "xml/xmlParser.cpp"
#include "default/default.cpp"
#include "unitExpression.cpp"
#include "interfaceElement.cpp"

namespace SplitGui {

    Interface::Interface() {
        SPLITGUI_PROFILE;


    };
    
    Interface::~Interface() {
        SPLITGUI_PROFILE;

        if (interfaceElement) {
            interfaceElement->cleanup();
            delete interfaceElement;
        }
    }

    Result Interface::parseXml(std::string& data) {
        SPLITGUI_PROFILE;

        XmlParser parser;
        ResultValue<InterfaceElement*> parseRet = parser.parse(data);
        TRYD(parseRet);

        setInterfaceElement(parseRet.value);

        return Result::eSuccess;
    }

    void Interface::submitGraphics(Graphics& graphics) {
        SPLITGUI_PROFILE;

        pGraphics = &graphics;
    }

    Result Interface::update() {
        SPLITGUI_PROFILE;

        return interfaceElement->update();
    }

    Result Interface::instance() {
        SPLITGUI_PROFILE;

        if (!pGraphics) {
            return Result::eMissingGraphics;
        }

        interfaceElement->setGraphics(pGraphics);
        return interfaceElement->instance();
    }

    void Interface::setViewport(RectObj viewport) {
        SPLITGUI_PROFILE;

        interfaceElement->extent = viewport;
    }

    void Interface::setInterfaceElement(InterfaceElement* data) {
        SPLITGUI_PROFILE;

        if (interfaceElement) {
            Logger::warn("Interface data is being overwritten this can cause memory leaks if unhandled");
        }

        interfaceElement = data;
    }

    void Interface::attachEventHandler(EventHandler& handler) {
        SPLITGUI_PROFILE;

        handler.attachInterface(this);
        pEventHandler = &handler;
    }

    InterfaceElement* Interface::getInterfaceElement() {
        SPLITGUI_PROFILE;

        return interfaceElement;
    }

    std::vector<InterfaceElement*> Interface::searchByReference(std::string reference) {
        SPLITGUI_PROFILE;

        return interfaceElement->searchByReference(reference);
    }
}