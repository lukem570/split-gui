#include "xmlParser.hpp"

namespace SplitGui {

    ResultValue<InterfaceElement*> XmlParser::parse(const std::string& xmlInput) {
        file = xmlInput;
        return parse(0);
    }

    ResultValue<InterfaceElement*> XmlParser::parse(int depth) {

        ResultValue<XmlToken> ___tokenRes = nextToken();
        TRYD(___tokenRes);
        XmlToken token = ___tokenRes.value;

        do {
            Begin:

            if (token.value != "<") {
                return Result::eInvalidToken;
            }

            ResultValue<XmlToken> __tokenRes = nextToken();
            TRYD(__tokenRes);
            token = __tokenRes.value;

            if (token.value == "split") {
                
                Default::Split* newSplit = new Default::Split();

                ResultValue<XmlToken> _tokenRes = nextToken();
                TRYD(_tokenRes);
                token = _tokenRes.value;

                while(token.type == XmlTokenType::eText) {
                    TRYR(splitRes, handleSplitParameters(newSplit, token));

                    ResultValue<XmlToken> tokenRes = nextToken();
                    TRYD(tokenRes);
                    token = tokenRes.value;
                }

                if (token.type == XmlTokenType::eTagClose) {
                    return Result::eInvalidPrematureClosure;
                }

                ResultValue<InterfaceElement*> parseRet1 = parse(depth + 1);
                TRYD(parseRet1);
                newSplit->addChild(parseRet1.value);
                
                ResultValue<InterfaceElement*> parseRet2 = parse(depth + 1);
                TRYD(parseRet2);
                newSplit->addChild(parseRet2.value);

                nextToken();
                nextToken();
                nextToken();

                return newSplit;
            }

            if (token.value == "list") {

            }

            if (token.value == "box") {

            }

            if (token.value == "overlay") {

            }

            if (token.value == "mask") {

            }

            if (token.value == "transform") {

            }

            if (token.value == "rect") {

                Default::Rect* newRect = new Default::Rect();

                ResultValue<XmlToken> _tokenRes = nextToken();
                TRYD(_tokenRes);
                token = _tokenRes.value;

                while(token.type == XmlTokenType::eText) {
                    TRYR(rectRes, handleRectParameters(newRect, token));

                    ResultValue<XmlToken> tokenRes = nextToken();
                    TRYD(tokenRes);
                    token = tokenRes.value;
                }

                if (token.type != XmlTokenType::eTagClose) {
                    return Result::eInvalidPrematureClosure;
                }

                return newRect;
            }

            if (token.value == "scene") {

                Default::SceneElement* newScene = new Default::SceneElement();

                ResultValue<XmlToken> _tokenRes = nextToken();
                TRYD(_tokenRes);
                token = _tokenRes.value;

                while(token.type == XmlTokenType::eText) {
                    TRYR(sceneRes, handleSceneParameters(newScene, token));

                    ResultValue<XmlToken> tokenRes = nextToken();
                    TRYD(tokenRes);
                    token = tokenRes.value;
                }

                if (token.type != XmlTokenType::eTagClose) {
                    return Result::eInvalidPrematureClosure;
                }

                return newScene;
            }

            if (token.value == "text") {

            }

            if (token.value == "media") {

            }

            if (token.value == "binding") {

            }

            if (token.value == "meta") {

                Default::Meta* newMeta = new Default::Meta();

                ResultValue<XmlToken> _tokenRes = nextToken();
                TRYD(_tokenRes);
                token = _tokenRes.value;

                while(token.type == XmlTokenType::eText) {
                    TRYR(metaRes, handleMetaParameters(newMeta, token));

                    ResultValue<XmlToken> tokenRes = nextToken();
                    TRYD(tokenRes);
                    token = tokenRes.value;
                }

                if (token.type != XmlTokenType::eTagClose) {
                    return Result::eInvalidPrematureClosure;
                }

                ResultValue<XmlToken> tokenRes_ = nextToken();
                TRYD(tokenRes_);
                token = tokenRes_.value;

                goto Begin;
            }

            return Result::eInvalidTag;

        } while (token.type != XmlTokenType::eEndOfFile && depth != 0);

        return Result::eInvalidXml;
    }

}