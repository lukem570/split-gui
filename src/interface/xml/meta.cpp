#include "xmlParser.hpp"

namespace SplitGui {
    Result XmlParser::handleMetaParameters(Default::Meta* split, XmlToken& token) {

        if (token.value == "version") {
            nextToken();
            nextToken();
            // TODO:
            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}