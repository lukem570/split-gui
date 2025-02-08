#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleOverlayTag() {
        Default::Overlay* newOverlay = new(std::nothrow) Default::Overlay();

        if (!newOverlay) {
            return Result::eHeapAllocFailed;
        }

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(overlayRes, handleOverlayParameters(newOverlay, token));
        }

        if (token.type != XmlTokenType::eTagOpen) {
            return Result::eInvalidPrematureClosure;
        }

        ResultValue<bool> closingTagRes = checkClosingTag("overlay");
        TRYD(closingTagRes);
        bool closingTag = closingTagRes.value;

        while (!closingTag) {
            ResultValue<InterfaceElement*> parseRes = parse();
            TRYD(parseRes);

            newOverlay->addChild(parseRes.value);

            ResultValue<bool> closingTagRes = checkClosingTag("overlay");
            TRYD(closingTagRes);
            closingTag = closingTagRes.value;
        }

        return newOverlay;
    }

    inline Result XmlParser::handleOverlayParameters(Default::Overlay* overlay, XmlToken& token) {

        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)overlay, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}