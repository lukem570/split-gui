#include "xmlParser.hpp"

namespace SplitGui {
    inline void XmlParser::fillHandleTagInterfaces(std::unordered_map<std::string, HandleTagInterface>& map) { 
        map["binding"]   = &SplitGui::XmlParser::handleBindingTag;
        map["box"]       = &SplitGui::XmlParser::handleBoxTag;
        map["list"]      = &SplitGui::XmlParser::handleListTag;
        map["meta"]      = &SplitGui::XmlParser::handleMetaTag;
        map["overlay"]   = &SplitGui::XmlParser::handleOverlayTag;
        map["rect"]      = &SplitGui::XmlParser::handleRectTag;
        map["scene"]     = &SplitGui::XmlParser::handleSceneTag;
        map["split"]     = &SplitGui::XmlParser::handleSplitTag;
        map["text"]      = &SplitGui::XmlParser::handleTextTag;
        map["transform"] = &SplitGui::XmlParser::handleTransformTag;
    }

    XmlParser::XmlParser() {
        fillHandleTagInterfaces(handleTagInterfaces);
    }
}

