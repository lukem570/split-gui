#include <splitgui/interface.hpp>

#include "xml/xmlParser.cpp"
#include "default/default.cpp"

namespace SplitGui {

    Interface::Interface() {

    };
    
    Interface::~Interface() {

    }

    void Interface::parseXml(std::string& data) {
        XMLParser parser(data);
        parser.tokenize();
        
    }

    InterfaceElement* Interface::getInterfaceElement() {
        return Interface::interfaceElement;
    }

    void Interface::setInterfaceElement(InterfaceElement& data) {
        if (Interface::interfaceElement) {
            printf("WARN: interface data is being overwritten this can cause memory leaks if unhandled!\n");
        }

        Interface::interfaceElement = &data;
    }

    void InterfaceElement::instance() {
        if (maxChildren < children.size()) {
            printf("ERROR: '%s' element has too many children", name.c_str());
            fflush(stdout);
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
}