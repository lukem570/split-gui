#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleTransformTag() {
        Default::Transform* newTransform = new Default::Transform();

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(transformRes, handleTransformParameters(newTransform, token));
        }

        if (token.type != XmlTokenType::eTagOpen) {
            return Result::eInvalidPrematureClosure;
        }

        ResultValue<bool> closingTagRes = checkClosingTag("transform");
        TRYD(closingTagRes);
        bool closingTag = closingTagRes.value;

        while (!closingTag) {
            ResultValue<InterfaceElement*> parseRes = parse();
            TRYD(parseRes);

            newTransform->addChild(parseRes.value);

            ResultValue<bool> closingTagRes = checkClosingTag("transform");
            TRYD(closingTagRes);
            closingTag = closingTagRes.value;
        }

        return newTransform;
    }

    inline Result XmlParser::handleTransformParameters(Default::Transform* transform, XmlToken& token) {
        return Result::eInvalidSetting;
    }
}