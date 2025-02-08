#include "xmlParser.hpp"

namespace SplitGui {
    ResultValue<InterfaceElement*> XmlParser::handleSceneTag() {
        Default::SceneElement* newScene = new(std::nothrow) Default::SceneElement();

        if (!newScene) {
            return Result::eHeapAllocFailed;
        }

        ResultValue<XmlToken> tokenRes = nextToken();
        TRYD(tokenRes);
        XmlToken token = tokenRes.value;

        while (token.type == XmlTokenType::eText) {
            TRYR(sceneRes, handleSceneParameters(newScene, token));
        }

        if (token.type != XmlTokenType::eTagClose) {
            return Result::eInvalidPrematureClosure;
        }

        return newScene;
    }

    Result XmlParser::handleSceneParameters(Default::SceneElement* scene, XmlToken& token) {

        ResultValue<bool> defaultRes = handleDefaultParameters((InterfaceElement*)scene, token);
        TRYD(defaultRes);
        if (defaultRes.value) {
            return Result::eSuccess;
        }

        if (token.value == "number") {
            ResultValue<XmlToken> attributeTokenRes = nextToken();
            TRYD(attributeTokenRes);
            token = attributeTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            ResultValue<XmlToken> valueTokenRes = nextToken();
            TRYD(valueTokenRes);
            token = valueTokenRes.value;
            ASSERT_ATTRIBUTE(token);

            scene->setSceneNumber(std::atoi(token.value.c_str()));

            ResultValue<XmlToken> finalTokenRes = nextToken();
            TRYD(finalTokenRes);
            token = finalTokenRes.value;

            return Result::eSuccess;
        }

        return Result::eInvalidSetting;
    }
}