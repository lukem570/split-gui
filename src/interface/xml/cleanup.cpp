#include "xmlParser.hpp"

namespace SplitGui {
    XmlParser::~XmlParser() {
        for (std::unordered_map<std::string, BindPointContainer>::iterator i = bindings.begin(); i != bindings.end(); i++) {
            if(!i->second.hasOwner) {
                delete i->second.node;
            }
        }
    }
}