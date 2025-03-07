#include <splitgui/interface.hpp>

#include "xml/xmlParser.cpp"
#include "default/default.cpp"
#include "unitExpression.cpp"
#include "interfaceElement.cpp"

namespace SplitGui {

    Interface::Interface() {

    };
    
    Interface::~Interface() {
        if (interfaceElement) {
            interfaceElement->cleanup();
            delete interfaceElement;
        }
    }

    Result Interface::parseXml(std::string& data) {
        XmlParser parser;
        ResultValue<InterfaceElement*> parseRet = parser.parse(data);
        TRYD(parseRet);

        setInterfaceElement(parseRet.value);

        return Result::eSuccess;
    }

    void Interface::submitGraphics(Graphics& graphics) {
        pGraphics = &graphics;
    }

    Result Interface::update() {
        return interfaceElement->update();
    }

    Result Interface::instance() {
        if (!pGraphics) {
            return Result::eMissingGraphics;
        }

        interfaceElement->setGraphics(pGraphics);
        return interfaceElement->instance();
    }

    void Interface::setViewport(RectObj viewport) {
        interfaceElement->extent = viewport;
    }

    void Interface::setInterfaceElement(InterfaceElement* data) {
        if (interfaceElement) {
            printf("WARN: interface data is being overwritten this can cause memory leaks if unhandled!\n");
        }

        interfaceElement = data;
    }

    void Interface::attachEventHandler(EventHandler& handler) {
        handler.attachInterface(this);
        pEventHandler = &handler;
    }

    InterfaceElement* Interface::getInterfaceElement() {
        return interfaceElement;
    }

    std::vector<InterfaceElement*> Interface::searchByReference(std::string reference) {
        return interfaceElement->searchByReference(reference);
    }
}