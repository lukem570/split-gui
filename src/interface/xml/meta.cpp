#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleMetaTag() {
        Default::Meta* newMeta = new Default::Meta();

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(metaRes, handleMetaParameters(newMeta, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        ResultValue<XmlToken> endTokenRes = nextToken();
        TRYD(endTokenRes);
        token = endTokenRes.value;

        return newMeta;
    }

    Result XmlParser::handleMetaParameters(Default::Meta* meta, XmlToken& token) {

        if (token.value == "version") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            // TODO:
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}