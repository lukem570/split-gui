#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<bool> XmlParser::handleDefaultParameters(InterfaceElement* element, XmlToken& token) {

        if (token.value == "ref") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            element->setRef(token.value);

            ResultValue<XmlToken> endTokenRes = nextToken();
            TRYD(endTokenRes);
            token = endTokenRes.value;

            return true;
        }

        if (token.value == "depth") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            element->setDepth(atoi(token.value.c_str()));

            ResultValue<XmlToken> endTokenRes = nextToken();
            TRYD(endTokenRes);
            token = endTokenRes.value;

            return true;
        }

        return false;
    }
}