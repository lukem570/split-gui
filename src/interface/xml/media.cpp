#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleMediaTag() {
        Default::Media* newMedia = new Default::Media();

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
        return Result::eInvalidSetting;
    }
}