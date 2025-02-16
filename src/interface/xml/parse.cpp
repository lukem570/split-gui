#include "xmlParser.hpp"

namespace SplitGui {

    ResultValue<InterfaceElement*> XmlParser::parse(const std::string& xmlInput) {
        file = xmlInput;
        return parse();
    }

    ResultValue<InterfaceElement*> XmlParser::parse() {

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        if (token.type != XmlTokenType::eTagOpen) {
            return Result::eInvalidToken;
        }

        while (token.type != XmlTokenType::eEndOfFile) {

            ResultValue<XmlToken> nameTokenRes = nextToken();
            TRYD(nameTokenRes);
            token = nameTokenRes.value;
            
            if (token.value == "meta") { // TODO: fix
                ResultValue<InterfaceElement*> metaRet = handleMetaTag();
                TRYD(metaRet);
                delete metaRet.value;
                continue;
            }
            
            if (token.value == "binding") {
                ResultValue<InterfaceElement*> bindingRet = handleBindingTag();
                TRYD(bindingRet);
                
                Default::Binding* binding = (Default::Binding*)bindingRet.value;
                
                std::string alias  = binding->getAlias();
                std::string source = binding->getSource();
                
                if (alias.empty() || source.empty()) {
                    return Result::eInvalidType;
                }
                
                std::ifstream pageFile(source);
                
                if (!pageFile.is_open()) {
                    return Result::eFailedToOpenFile;
                }
                
                std::stringstream buffer;
                buffer << pageFile.rdbuf();
                std::string page = buffer.str();
                
                XmlParser parser;
                ResultValue<InterfaceElement*> parseRet = parser.parse(page);
                TRYD(parseRet);
                
                bindings[alias].node = parseRet.value;
                
                if(handleTagInterfaces.find(alias) != handleTagInterfaces.end()) {
                    return Result::eInvalidTag;
                }
                
                delete bindingRet.value;
                continue;
            }
            
            if (bindings.find(token.value) != bindings.end()) {
            
                Default::BindPoint* newBindPoint = new Default::BindPoint();
                
                newBindPoint->setBindPoint(bindings[token.value].node);
                newBindPoint->setOwnership(!bindings[token.value].hasOwner);
                
                bindings[token.value].hasOwner = true;
                
                ResultValue<XmlToken> endTagTokenRes = nextToken();
                TRYD(endTagTokenRes);
                token = endTagTokenRes.value;

                if (token.type != XmlTokenType::eTagClose) {
                    return Result::eInvalidPrematureClosure;
                }

                return (InterfaceElement*)newBindPoint;
            }
            
            if (handleTagInterfaces.find(token.value) == handleTagInterfaces.end()) {
                return Result::eInvalidTag;
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