#include "xmlParser.hpp"

namespace SplitGui {
    Result XmlParser::handleSceneParameters(Default::SceneElement* scene, XmlToken& token) {

        if (token.value == "number") {
            nextToken();
            nextToken();

            // TODO:

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}