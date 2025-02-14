#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleTextTag() {

        
        Default::Text* newText = new(std::nothrow) Default::Text();
        
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
            SPLITGUI_LOG("Value: %s", token.value.c_str());
            
            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;
            
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}