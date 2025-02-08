#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleMediaTag() {
        Default::Media* newMedia = new(std::nothrow) Default::Media();

        if (!newMedia) {
            return Result::eHeapAllocFailed;
        }

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(mediaRes, handleMediaParameters(newMedia, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        return newMedia;
    }

    inline Result XmlParser::handleMediaParameters(Default::Media* media, XmlToken& token) {

        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)media, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}