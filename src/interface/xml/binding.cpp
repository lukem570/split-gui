#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleBindingTag() {
        Default::Binding* newBinding = new Default::Binding();

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(bindingRes, handleBindingParameters(newBinding, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        ResultValue<XmlToken> endTokenRes = nextToken();
        TRYD(endTokenRes);
        token = endTokenRes.value;

        return newBinding;
    }

    inline Result XmlParser::handleBindingParameters(Default::Binding* binding, XmlToken& token) {

        if (token.value == "alias") {

            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            binding->setAlias(token.value);

            ResultValue<XmlToken> endTokenRes = nextToken();
            TRYD(endTokenRes);
            token = endTokenRes.value;

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}