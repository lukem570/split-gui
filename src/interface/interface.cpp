#include <splitgui/interface.hpp>
#include "xmlParser.cpp"

namespace SplitGui {

    Interface::Interface() {

    };
    
    Interface::~Interface() {
        if(Interface::interfaceData) {
            cleanupInterfaceData(Interface::interfaceData);
        }
    }

    void Interface::parseXml(std::string& data) {
        //XmlParser::tokenize(data);

        
    }

    InterfaceData* Interface::getInterfaceData() {
        return Interface::interfaceData;
    }

    void Interface::setInterfaceData(InterfaceData& data) {
        if (Interface::interfaceData) {
            printf("WARN: interface data is being overwritten this can cause memory leaks if unhandled!\n");
        }

        Interface::interfaceData = &data;
    }

    void Interface::cleanupInterfaceData(InterfaceData* element) {
        for (int i = 0; i < element->children.size(); i++) {
            cleanupInterfaceData(element->children[i]);
            delete element->children[i];
        }
    }
}