#include <splitgui/interface.hpp>

#include "xml/xmlParser.cpp"
#include "default/default.cpp"
#include "unitExpression.cpp"

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
        XMLParser parser(data);
        ResultValue<InterfaceElement*> parseRet = parser.parse();

        TRYD(parseRet);

        setInterfaceElement(parseRet.value);

        return Result::eSuccess;
    }

    void Interface::submitGraphics(Graphics& graphics) {
        pGraphics = &graphics;
    }

    void Interface::update() {
        interfaceElement->update();
    }

    void Interface::instance() {
        if (!pGraphics) {
            printf("WARN: interface requires graphics to call 'update'\n");
            return;
        }

        interfaceElement->setGraphics(pGraphics);
        interfaceElement->instance();
    }

    void Interface::setViewport(RectObj viewport) {
        interfaceElement->extent = viewport;
    }

    InterfaceElement* Interface::getInterfaceElement() {
        return Interface::interfaceElement;
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

    Result InterfaceElement::instance() {
        if (maxChildren < children.size()) {
            return Result::eInvalidNumberOfChildren;
        }

        for (int i = 0; i < children.size(); i++) {
            if (children[i]->type == InterfaceElementType::eMeta    ||
                children[i]->type == InterfaceElementType::eBinding) {

                continue;
            }

            children[i]->setGraphics(pGraphics);
            children[i]->setExtent(extent);
            children[i]->instance();
            break;
        }

        return Result::eSuccess;
    }

    void InterfaceElement::update() {
        for (int i = 0; i < children.size(); i++) {
            if (children[i]->type == InterfaceElementType::eMeta    ||
                children[i]->type == InterfaceElementType::eBinding) {

                continue;
            }

            children[i]->setExtent(extent);
            children[i]->update();
            break;
        }
    }

    void InterfaceElement::addChild(InterfaceElement* child) {
        children.push_back(child);
    }

    void InterfaceElement::setSize(IVec2 size) {
        extent.width  = size.x;
        extent.height = size.y;
    }

    void InterfaceElement::setPosition(IVec2 position) {
        extent.x = position.x;
        extent.y = position.y;
    }

    void InterfaceElement::setExtent(RectObj extentIn) {
        extent = extentIn;
    }

    void InterfaceElement::setGraphics(Graphics* pGraphicsIn) {
        pGraphics = pGraphicsIn;
    }

    void InterfaceElement::cleanup() {
        for (int i = 0; i < children.size(); i++) {
            children[i]->cleanup();
            delete children[i];
        }
    }
}