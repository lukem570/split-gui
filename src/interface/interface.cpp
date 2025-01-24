#include <splitgui/interface.hpp>

#include "xml/xmlParser.cpp"
#include "default/default.cpp"

namespace SplitGui {

    Interface::Interface() {

    };
    
    Interface::~Interface() {
        if (interfaceElement) {
            interfaceElement->cleanup();
            delete interfaceElement;
        }
    }

    void Interface::parseXml(std::string& data) {
        XMLParser parser(data);
        setInterfaceElement(parser.parse());
    }

    void Interface::submitGraphics(Graphics& graphics) {
        pGraphics = &graphics;
    }

    void Interface::update() {
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

    void InterfaceElement::instance() {
        if (maxChildren < children.size()) {
            printf("ERROR: '%s' element has too many children", name.c_str());
            throw;
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