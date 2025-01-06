#include <splitgui/interface.hpp>

#include "xml/xmlParser.cpp"

namespace SplitGui {

    Interface::Interface() {

    };
    
    Interface::~Interface() {
        if(Interface::interfaceElement) {
            cleanupInterfaceElement(Interface::interfaceElement);
        }
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

    void Interface::cleanupInterfaceElement(InterfaceElement* element) {
        for (int i = 0; i < element->children.size(); i++) {
            cleanupInterfaceElement(element->children[i]);
            delete element->children[i];
        }
    }
}