#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleTextTag() {
        Default::Text* newText = new(std::nothrow) Default::Text();
        ma::track(newText, "text");
        
        if (!newText) {
            return Result::eHeapAllocFailed;
        }
        
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
        
        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)text, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }
        
        if (token.value == "value") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);
            
            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);
            
            text->setText(token.value);
            
            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;
            
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

            TRYR(setColorRes, text->setColor(token.value));

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        if (token.value == "size") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            text->setSize(std::atoi(token.value.c_str()));

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        if (token.value == "horizontalAnchor") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            if (token.value == "left") {
                text->setHorizontalAnchor(HorizontalAnchor::eLeft);
            } else if (token.value == "center") {
                text->setHorizontalAnchor(HorizontalAnchor::eCenter);
            } else if (token.value == "right") {
                text->setHorizontalAnchor(HorizontalAnchor::eRight);
            } else {
                return Result::eInvalidToken;
            }

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        if (token.value == "verticalAnchor") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            if (token.value == "top") {
                text->setVerticalAnchor(VerticalAnchor::eTop);
            } else if (token.value == "center") {
                text->setVerticalAnchor(VerticalAnchor::eCenter);
            } else if (token.value == "bottom") {
                text->setVerticalAnchor(VerticalAnchor::eBottom);
            } else {
                return Result::eInvalidToken;
            }

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}