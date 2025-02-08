#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleListTag() {
        Default::List* newList = new(std::nothrow) Default::List();

        if (!newList) {
            return Result::eHeapAllocFailed;
        }

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(listRes, handleListParameters(newList, token));
        }

        if (token.type != XmlTokenType::eTagOpen) {
            return Result::eInvalidPrematureClosure;
        }

        ResultValue<bool> closingTagRes = checkClosingTag("list");
        TRYD(closingTagRes);
        bool closingTag = closingTagRes.value;

        while (!closingTag) {
            ResultValue<InterfaceElement*> parseRes = parse();
            TRYD(parseRes);

            newList->addChild(parseRes.value);

            ResultValue<bool> closingTagRes = checkClosingTag("list");
            TRYD(closingTagRes);
            closingTag = closingTagRes.value;
        }

        return newList;
    }

    inline Result XmlParser::handleListParameters(Default::List* list, XmlToken& token) {

        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)list, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        if (token.value == "direction") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            if (token.value == "vertical") {
                list->setVertical(true);
            } else if (token.value == "horizontal") {
                list->setVertical(false);
            } else {
                return Result::eInvalidDirection;
            }

            ResultValue<XmlToken> endTokenRes = nextToken();
            TRYD(endTokenRes);
            token = endTokenRes.value;

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}