#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <stack>

namespace SplitGui {

    class XMLParser {
        public:
            XMLParser(const std::string& xmlInput) : input(xmlInput), pos(0) {}

            Result handleSplitParameters(Default::Split* split, XmlToken& token) {

                if (token.value == "position") {
                    nextToken();
                    token = nextToken();

                    printf("position: %s\n", token.value.c_str());

                    TRYR(split->setPosition(token.value));
                    
                    return Result::eSuccess;
                }

                if (token.value == "direction") { // isVertical
                    nextToken();
                    token = nextToken();
                    
                    if (token.value == "vertical") {
                        split->setVertical(true);
                    } else if (token.value == "horizontal") {
                        split->setVertical(false);
                    } else {
                        return Result::eInvalidDirection;
                    }

                    return Result::eSuccess;
                }

                return Result::eInvalidSetting;
            }

            Result handleRectParameters(Default::Rect* rect, XmlToken& token) {

                if (token.value == "color") {
                    nextToken();
                    token = nextToken();

                    printf("color  str: %s\n", token.value.c_str());

                    IVec3 vec3;

                    std::string numbersPart = token.value.substr(6, token.value.size() - 7);

                    std::stringstream ss(numbersPart);

                    char comma;
                    ss >> vec3.x >> comma >> vec3.y >> comma >> vec3.z;

                    printf("rect color: ivec3(%d, %d, %d)\n", vec3.x, vec3.y, vec3.z);
                    
                    rect->setColor(vec3);

                    return Result::eSuccess;
                }

                return Result::eInvalidSetting;
            }

            Result handleMetaParameters(Default::Meta* split, XmlToken& token) {

                if (token.value == "version") {
                    nextToken();
                    nextToken();
                    // TODO:
                    return Result::eSuccess;
                }

                return Result::eInvalidSetting;
            }

            Result handleSceneParameters(Default::SceneElement* scene, XmlToken& token) {

                if (token.value == "number") {
                    nextToken();
                    token = nextToken();
                    
                    

                    return Result::eSuccess;
                }

                return Result::eInvalidSetting;
            }

            ResultValue<InterfaceElement*> parse(int depth = 0) {

                XmlToken token = nextToken();

                do {
                    Begin:

                    if (token.value != "<") {
                        return Result::eInvalidToken;
                    }

                    token = nextToken();

                    if (token.value == "split") {
                        
                        Default::Split* newSplit = new Default::Split();

                        token = nextToken();

                        while(token.type == XmlTokenType::eText) {
                            TRYR(handleSplitParameters(newSplit, token));
                            token = nextToken();
                        }

                        if (token.type == XmlTokenType::eTagClose) {
                            return Result::eInvalidPrematureClosure;
                        }

                        ResultValue<InterfaceElement*> parseRet1 = parse(depth + 1);
                        TRY(parseRet1);
                        newSplit->addChild(parseRet1.value);

                        ResultValue<InterfaceElement*> parseRet2 = parse(depth + 1);
                        TRY(parseRet2);
                        newSplit->addChild(parseRet2.value);

                        nextToken();
                        nextToken();
                        nextToken();

                        return newSplit;
                    }

                    if (token.value == "list") {

                    }

                    if (token.value == "box") {

                    }

                    if (token.value == "overlay") {

                    }

                    if (token.value == "mask") {

                    }

                    if (token.value == "transform") {

                    }

                    if (token.value == "rect") {

                        Default::Rect* newRect = new Default::Rect();

                        token = nextToken();

                        while(token.type == XmlTokenType::eText) {
                            TRYR(handleRectParameters(newRect, token));
                            token = nextToken();
                        }

                        if (token.type != XmlTokenType::eTagClose) {
                            return Result::eInvalidPrematureClosure;
                        }

                        return newRect;
                    }

                    if (token.value == "scene") {

                        Default::SceneElement* newScene = new Default::SceneElement();

                        token = nextToken();

                        while(token.type == XmlTokenType::eText) {
                            TRYR(handleSceneParameters(newScene, token));
                            token = nextToken();
                        }

                        if (token.type != XmlTokenType::eTagClose) {
                            return Result::eInvalidPrematureClosure;
                        }

                        return newScene;
                    }

                    if (token.value == "text") {

                    }

                    if (token.value == "media") {

                    }

                    if (token.value == "binding") {

                    }

                    if (token.value == "meta") {

                        Default::Meta* newMeta = new Default::Meta();

                        token = nextToken();

                        while(token.type == XmlTokenType::eText) {
                            TRYR(handleMetaParameters(newMeta, token));
                            token = nextToken();
                        }

                        if (token.type != XmlTokenType::eTagClose) {
                            return Result::eInvalidPrematureClosure;
                        }

                        token = nextToken();

                        goto Begin;
                    }

                    if (token.value == "root") {

                    }

                    return Result::eInvalidTag;

                } while (token.type != XmlTokenType::eEndOfFile && depth != 0);

                return Result::eInvalidXml;
            }

        private:
            std::string input;
            int pos;
            
            char currentChar() {
                if (pos < input.size()) {
                    return input[pos];
                }
                return '\0';
            }
            
            void advance() {
                if (pos < input.size()) {
                    pos++;
                }
            }

            XmlToken nextToken() {
                while (currentChar() == '\n' || 
                       currentChar() == '\t' || 
                       currentChar() == '\r' ||
                       currentChar() == ' '  ||
                       currentChar() == '/') {
                    advance();
                }

                if (currentChar() == '\0') {
                    return XmlToken{XmlTokenType::eEndOfFile, ""};
                }

                if (currentChar() == '<') {
                    advance();
                    if (currentChar() == '/') {
                        advance();
                        return XmlToken{XmlTokenType::eTagClose, "</"};
                    }
                    return XmlToken{XmlTokenType::eTagOpen, "<"};
                }

                if (currentChar() == '>') {
                    if (input[pos - 1] == '/') {
                        advance();
                        advance();
                        return XmlToken{XmlTokenType::eTagClose, "/>"};
                    }
                    advance();
                    return XmlToken{XmlTokenType::eTagOpen, ">"};
                }

                if (currentChar() == '=') {
                    advance();
                    return XmlToken{XmlTokenType::eAttribute, "="};
                }

                if (currentChar() == '"') {
                    advance();
                    int start = pos;
                    while (currentChar() != '"' && currentChar() != '\0') {
                        advance();
                    }
                    std::string value = input.substr(start, pos - start);
                    advance();
                    return XmlToken{XmlTokenType::eAttribute, value};
                }

                if (isalnum(currentChar())) {
                    int start = pos;
                    while (isalnum(currentChar()) || currentChar() == '-' || currentChar() == '_') {
                        advance();
                    }
                    return XmlToken{XmlTokenType::eText, input.substr(start, pos - start)};
                }

                std::stringstream err;
                err << "Invalid character: " << (int)currentChar();

                return XmlToken{XmlTokenType::eError, err.str()};
            }
    };
};