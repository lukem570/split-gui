#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleRectTag() {
        Default::Rect* newRect = new Default::Rect();

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(rectRes, handleRectParameters(newRect, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        return newRect;
    }

    Result XmlParser::handleRectParameters(Default::Rect* rect, XmlToken& token) {

        if (token.value == "color") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            IVec3 vec3;
            std::string numbersPart = token.value.substr(6, token.value.size() - 7);
            std::stringstream ss(numbersPart);

            char comma;
            ss >> vec3.x >> comma >> vec3.y >> comma >> vec3.z;

            rect->setColor(vec3);

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}