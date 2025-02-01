#include "xmlParser.hpp"

namespace SplitGui {
    char XmlParser::currentChar() {
        if (index < file.size()) {
            return file[index];
        }
        return '\0';
    }

    void XmlParser::advance() {
        if (index < file.size()) {
            index++;
        }
    }

    ResultValue<XmlToken> XmlParser::nextToken() {
        while ((std::isspace(currentChar()) || currentChar() == '/') && currentChar() != '\0') {
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
            if (file[index - 1] == '/') {
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
            int start = index;

            while (currentChar() != '"' && currentChar() != '\0') {
                advance();
            }

            std::string value = file.substr(start, index - start);
            advance();

            return XmlToken{XmlTokenType::eAttribute, value};
        }

        if (std::isalnum(currentChar())) {
            int start = index;

            while (std::isalnum(currentChar()) || currentChar() == '-' || currentChar() == '_') {
                advance();
            }

            return XmlToken{XmlTokenType::eText, file.substr(start, index - start)};
        }

        return Result::eInvalidToken;
    }
}