#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleTextTag() {
        Default::Text* newText = new Default::Text();

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(textRes, handleTextParameters(newText, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        return newText;
    }

    inline Result XmlParser::handleTextParameters(Default::Text* text, XmlToken& token) {
        return Result::eInvalidSetting;
    }
}