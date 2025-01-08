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
        if (InterfaceElement::maxChildren < InterfaceElement::children.size()) {
            printf("ERROR: '%s' element has too many children", InterfaceElement::name.c_str());
            throw;
        }

        for (int i = 0; i < InterfaceElement::children.size(); i++) {
            if (InterfaceElement::children[i]->type == InterfaceElementType::eMeta    ||
                InterfaceElement::children[i]->type == InterfaceElementType::eBinding) {

                continue;
            }

            printf("%s\n", InterfaceElement::children[i]->name.c_str());

            InterfaceElement::children[i]->setGraphics(InterfaceElement::pGraphics);
            InterfaceElement::children[i]->setExtent(InterfaceElement::extent);
            InterfaceElement::children[i]->instance();
            break;
        }
    }

    void InterfaceElement::addChild(InterfaceElement* child) {
        InterfaceElement::children.push_back(child);
    }

    void InterfaceElement::setSize(IVec2 size) {
        InterfaceElement::extent.width  = size.x;
        InterfaceElement::extent.height = size.y;
    }

    void InterfaceElement::setPosition(IVec2 position) {
        InterfaceElement::extent.x = position.x;
        InterfaceElement::extent.y = position.y;
    }

    void InterfaceElement::setExtent(RectObj extent) {
        InterfaceElement::extent = extent;
    }

    void InterfaceElement::setGraphics(Graphics* pGraphics) {
        InterfaceElement::pGraphics = pGraphics;
    }
}