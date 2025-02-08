#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleBoxTag() {
        Default::Box* newBox = new(std::nothrow) Default::Box();

        if (!newBox) {
            return Result::eHeapAllocFailed;
        }

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(boxRes, handleBoxParameters(newBox, token));
        }

        if (token.type != XmlTokenType::eTagOpen) {
            return Result::eInvalidPrematureClosure;
        }

        ResultValue<bool> closingTagRes = checkClosingTag("box");
        TRYD(closingTagRes);
        bool closingTag = closingTagRes.value;

        while (!closingTag) {
            ResultValue<InterfaceElement*> parseRes = parse();
            TRYD(parseRes);

            newBox->addChild(parseRes.value);

            ResultValue<bool> closingTagRes = checkClosingTag("box");
            TRYD(closingTagRes);
            closingTag = closingTagRes.value;
        }

        return newBox;
    }

    inline Result XmlParser::handleBoxParameters(Default::Box* box, XmlToken& token) {
        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)box, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}