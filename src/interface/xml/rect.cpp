#include "xmlParser.hpp"

namespace SplitGui {
    Result XmlParser::handleRectParameters(Default::Rect* rect, XmlToken& token) {

        if (token.value == "color") {
            nextToken();

            ResultValue<XmlToken> tokenRes = nextToken();
            TRYD(tokenRes);
            token = tokenRes.value;

            printf("color  str: %s\n", token.value.c_str());

            IVec3 vec3;

            std::string numbersPart = token.value.substr(6, token.value.size() - 7);

            std::stringstream ss(numbersPart);

            char comma;
            ss >> vec3.x >> comma >> vec3.y >> comma >> vec3.z;

            printf("rect color: ivec3(%d, %d, %d)\n", vec3.x, vec3.y, vec3.z);
            
            rect->setColor(vec3);

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}