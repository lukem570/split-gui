#include "xmlParser.hpp"

namespace SplitGui {
    Result XmlParser::handleSplitParameters(Default::Split* split, XmlToken& token) {

        if (token.value == "position") {
            nextToken();

            ResultValue<XmlToken> tokenRes = nextToken();
            TRYD(tokenRes);
            token = tokenRes.value;

            printf("position: %s\n", token.value.c_str());

            TRYR(positionRes, split->setPosition(token.value));
            
            return Result::eSuccess;
        }

        if (token.value == "direction") { // isVertical
            nextToken();

            ResultValue<XmlToken> tokenRes = nextToken();
            TRYD(tokenRes);
            token = tokenRes.value;
            
            if (token.value == "vertical") {
                split->setVertical(true);
            } else if (token.value == "horizontal") {
                split->setVertical(false);
            } else {
                return Result::eInvalidDirection;
            }

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}