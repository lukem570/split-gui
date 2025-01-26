#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <stack>

namespace SplitGui {

    class XMLParser {
        public:
            XMLParser(const std::string& xmlInput) : input(xmlInput), pos(0) {
                // create root node
            }

            void handleSplitParameters(Default::Split* split, XmlToken& token) {

                if (token.value == "position") {
                    nextToken();
                    nextToken();
                    // TODO:
                    return;
                }

                if (token.value == "direction") { // isVertical
                    nextToken();
                    token = nextToken();
                    
                    if (token.value == "vertical") {
                        split->setVertical(true);
                    } else if (token.value == "horizontal") {
                        split->setVertical(false);
                    } else {
                        printf("ERROR: invalid direction %s\n", token.value.c_str());
                        throw;
                    }

                    return;
                }

                throw;
            }

            void handleRectParameters(Default::Rect* rect, XmlToken& token) {

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

                    return;
                }

                throw;
            }

            void handleMetaParameters(Default::Meta* split, XmlToken& token) {

                if (token.value == "version") {
                    nextToken();
                    nextToken();
                    // TODO:
                    return;
                }

                throw;
            }

            void handleSceneParameters(Default::SceneElement* scene, XmlToken& token) {

                if (token.value == "number") {
                    nextToken();
                    token = nextToken();
                    
                    

                    return;
                }

                throw;
            }

            InterfaceElement* parse(int depth = 0) {

                XmlToken token = nextToken();

                do {
                    Begin:

                    if (token.value != "<") {
                        printf("ERROR: invalid tag %s\n", token.value.c_str());
                        throw;
                    }

                    token = nextToken();

                    if (token.value == "split") {
                        
                        Default::Split* newSplit = new Default::Split();

                        token = nextToken();

                        while(token.type == XmlTokenType::eText) {
                            handleSplitParameters(newSplit, token);
                            token = nextToken();
                        }

                        if (token.type == XmlTokenType::eTagClose) {
                            printf("ERROR: split cannot be closed prematurely it must have 2 children\n");
                            fflush(stdout);
                            throw;
                        }

                        newSplit->addChild(parse(depth + 1));
                        newSplit->addChild(parse(depth + 1));

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
                            handleRectParameters(newRect, token);
                            token = nextToken();
                        }

                        if (token.type != XmlTokenType::eTagClose) {
                            printf("ERROR: rect must be closed prematurely\n");
                            throw;
                        }

                        return newRect;
                    }

                    if (token.value == "scene") {

                        Default::SceneElement* newScene = new Default::SceneElement();

                        token = nextToken();

                        while(token.type == XmlTokenType::eText) {
                            handleSceneParameters(newScene, token);
                            token = nextToken();
                        }

                        if (token.type != XmlTokenType::eTagClose) {
                            printf("ERROR: scene must be closed prematurely\n");
                            throw;
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
                            handleMetaParameters(newMeta, token);
                            token = nextToken();
                        }

                        if (token.type != XmlTokenType::eTagClose) {
                            printf("ERROR: meta must be closed prematurely\n");
                            throw;
                        }

                        token = nextToken();

                        goto Begin;
                    }

                    if (token.value == "root") {

                    }

                    throw;

                } while (token.type != XmlTokenType::eEndOfFile && depth != 0);

                printf("Error: Invalid xml\n");

                throw;
                return nullptr;
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