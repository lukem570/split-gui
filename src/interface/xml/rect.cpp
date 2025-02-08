#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleRectTag() {
        SPLITGUI_LOG("Rect Created1");

        Default::Rect* newRect = new(std::nothrow) Default::Rect();

        SPLITGUI_LOG("Rect Created2");

        if (!newRect) {
            return Result::eHeapAllocFailed;
        }

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(rectRes, handleRectParameters(newRect, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        SPLITGUI_LOG("Rect Created");

        return (InterfaceElement*)newRect;
    }

    Result XmlParser::handleRectParameters(Default::Rect* rect, XmlToken& token) {

        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)rect, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        if (token.value == "color") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            TRYR(setColorRes, rect->setColor(token.value));
            printf("rect color: %s\n", token.value.c_str());
            fflush(stdout);

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}