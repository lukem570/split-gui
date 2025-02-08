#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleMaskTag() {
        Default::Mask* newMask = new(std::nothrow) Default::Mask();

        if (!newMask) {
            return Result::eHeapAllocFailed;
        }

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

        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)mask, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}