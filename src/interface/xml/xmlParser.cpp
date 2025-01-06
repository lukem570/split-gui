#include <string>
#include <vector>
#include <sstream>

namespace SplitGui {

    enum class TokenType {
        eTagOpen,
        eTagClose,
        eText,
        eAttribute,
        eEndOfFile,
        eError
    };

    struct Token {
        TokenType type;
        std::string value;
    };

    class XMLParser {
        public:
            XMLParser(const std::string& xmlInput) : input(xmlInput), pos(0) {
                // create root node
            }

            void tokenize() {
                Token token;
                while ((token = nextToken()).type != TokenType::eEndOfFile) {
                    switch (token.type) {
                        case TokenType::eTagOpen:

                            // < and >
                            break;

                        case TokenType::eTagClose:

                            // </ and />
                            break;

                        case TokenType::eText:

                            // text and numbers
                            break;

                        case TokenType::eAttribute:

                            // names and param names
                            break;

                        case TokenType::eError:

                            printf("Error: %s\n", token.value.c_str());
                            throw;
                    }
                }
            }

            void parseToSplitGuiInterface() {

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

            Token nextToken() {
                while (currentChar() == '\n' || 
                       currentChar() == '\t' || 
                       currentChar() == '\r' ||
                       currentChar() == ' '  ||
                       currentChar() == '/') {
                    advance();
                }

                if (currentChar() == '\0') {
                    return Token{TokenType::eEndOfFile, ""};
                }

                if (currentChar() == '<') {
                    advance();
                    if (currentChar() == '/') {
                        advance();
                        return Token{TokenType::eTagClose, "</"};
                    }
                    return Token{TokenType::eTagOpen, "<"};
                }

                if (currentChar() == '>') {
                    if (input[pos - 1] == '/') {
                        advance();
                        advance();
                        return Token{TokenType::eTagClose, "/>"};
                    }
                    advance();
                    return Token{TokenType::eTagOpen, ">"};
                }

                if (currentChar() == '=') {
                    advance();
                    return Token{TokenType::eAttribute, "="};
                }

                if (currentChar() == '"') {
                    advance();
                    int start = pos;
                    while (currentChar() != '"' && currentChar() != '\0') {
                        advance();
                    }
                    std::string value = input.substr(start, pos - start);
                    advance();
                    return Token{TokenType::eAttribute, value};
                }

                if (isalnum(currentChar())) {
                    int start = pos;
                    while (isalnum(currentChar()) || currentChar() == '-' || currentChar() == '_') {
                        advance();
                    }
                    return Token{TokenType::eText, input.substr(start, pos - start)};
                }

                std::stringstream err;
                err << "Invalid character: " << (int)currentChar();

                return Token{TokenType::eError, err.str()};
            }
    };
};