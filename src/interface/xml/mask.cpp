#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleMaskTag() {
        Default::Mask* newMask = new Default::Mask();

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(maskRes, handleMaskParameters(newMask, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        return newMask;
    }
    
    inline Result XmlParser::handleMaskParameters(Default::Mask* mask, XmlToken& token) {
        return Result::eInvalidSetting;
    }
}