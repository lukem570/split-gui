#include "xmlParser.hpp"

namespace SplitGui {

    ResultValue<InterfaceElement*> XmlParser::parse(const std::string& xmlInput) {
        file = xmlInput;
        return parse();
    }

    inline void XmlParser::fillHandleTagInterfaces(std::unordered_map<std::string, HandleTagInterface>& map) {
        map["binding"]   = &SplitGui::XmlParser::handleBindingTag;
        map["box"]       = &SplitGui::XmlParser::handleBoxTag;
        map["list"]      = &SplitGui::XmlParser::handleListTag;
        map["mask"]      = &SplitGui::XmlParser::handleMaskTag;
        map["media"]     = &SplitGui::XmlParser::handleMediaTag;
        map["meta"]      = &SplitGui::XmlParser::handleMetaTag;
        map["overlay"]   = &SplitGui::XmlParser::handleOverlayTag;
        map["rect"]      = &SplitGui::XmlParser::handleRectTag;
        map["scene"]     = &SplitGui::XmlParser::handleSceneTag;
        map["split"]     = &SplitGui::XmlParser::handleSplitTag;
        map["text"]      = &SplitGui::XmlParser::handleTextTag;
        map["transform"] = &SplitGui::XmlParser::handleTransformTag;
    }

    ResultValue<InterfaceElement*> XmlParser::parse() {

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        if (token.type != XmlTokenType::eTagOpen) {
            return Result::eInvalidToken;
        }

        std::unordered_map<std::string, HandleTagInterface> handleTagInterfaces;
        fillHandleTagInterfaces(handleTagInterfaces);

        while (token.type != XmlTokenType::eEndOfFile) {
        printf("begin2: %s, %d\n", token.value.c_str(), index);

            ResultValue<XmlToken> nameTokenRes = nextToken();
            TRYD(nameTokenRes);
            token = nameTokenRes.value;
            
            if (handleTagInterfaces.find(token.value) == handleTagInterfaces.end()) {
                return Result::eInvalidTag;
            }

            printf("parse: %s\n", token.value.c_str());

            if (token.value == "meta") { // TODO: fix
                ResultValue<InterfaceElement*> metaRet = handleMetaTag();
                TRYD(metaRet);
                delete metaRet.value;
                continue;
            }

            if (token.value == "binding") {
                ResultValue<InterfaceElement*> bindingRet = handleBindingTag();
                TRYD(bindingRet);
                delete bindingRet.value;
                continue;
            }

            return (this->*handleTagInterfaces[token.value])();
        } 

        return Result::eInvalidXml;
    }

    ResultValue<bool> XmlParser::checkClosingTag(std::string name) {
        int startIndex = index;

        ResultValue<XmlToken> closeTokenRes = nextToken();
        TRYD(closeTokenRes);
        XmlToken token = closeTokenRes.value;

        if (token.type != XmlTokenType::eTagClose) {
            index = startIndex;
            return false;
        }

        ResultValue<XmlToken> nameTokenRes = nextToken();
        TRYD(nameTokenRes);
        token = nameTokenRes.value;

        if (token.value != name) {
            index = startIndex;
            return false;
        }

        ResultValue<XmlToken> finalTokenRes = nextToken();
        TRYD(finalTokenRes);
        token = finalTokenRes.value;

        if (token.type != XmlTokenType::eTagOpen) {
            index = startIndex;
            return false;
        }
        
        return true;
    }
}