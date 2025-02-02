#ifndef SPLITGUI_XML_PARSER_HPP
#define SPLITGUI_XML_PARSER_HPP

#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include <string>
#include <cctype>
#include <unordered_map>

/*
#include "xmlParser.hpp"

namespace SplitGui {

}
*/

namespace SplitGui {

    #define ASSERT_ATTRIBUTE(token) if (token.type != XmlTokenType::eAttribute) { return Result::eInvalidToken; }

    class XmlParser;

    typedef SplitGui::ResultValue<SplitGui::InterfaceElement*> (SplitGui::XmlParser::*HandleTagInterface)();

    class XmlParser {
        public:
            XmlParser()  = default;
            ~XmlParser() = default;

            ResultValue<InterfaceElement*> parse(const std::string& xmlInput);
        
        private:
            std::string file;
            int         index = 0;

            char currentChar();
            void advance();

            ResultValue<XmlToken> nextToken();

            ResultValue<InterfaceElement*> parse();

            inline void fillHandleTagInterfaces(std::unordered_map<std::string, HandleTagInterface>& map);

            ResultValue<bool> checkClosingTag(std::string name);

            ResultValue<InterfaceElement*> handleBindingTag();
            ResultValue<InterfaceElement*> handleBoxTag();
            ResultValue<InterfaceElement*> handleListTag();
            ResultValue<InterfaceElement*> handleMaskTag();
            ResultValue<InterfaceElement*> handleMediaTag();
            ResultValue<InterfaceElement*> handleMetaTag();
            ResultValue<InterfaceElement*> handleOverlayTag();
            ResultValue<InterfaceElement*> handleRectTag();
            ResultValue<InterfaceElement*> handleSceneTag();
            ResultValue<InterfaceElement*> handleSplitTag();
            ResultValue<InterfaceElement*> handleTextTag();
            ResultValue<InterfaceElement*> handleTransformTag();

            inline Result handleBindingParameters(Default::Binding* binding, XmlToken& token);
            inline Result handleBoxParameters(Default::Box* box, XmlToken& token);
            inline Result handleListParameters(Default::List* list, XmlToken& token);
            inline Result handleMaskParameters(Default::Mask* mask, XmlToken& token);
            inline Result handleMediaParameters(Default::Media* media, XmlToken& token);
            inline Result handleMetaParameters(Default::Meta* meta, XmlToken& token);
            inline Result handleOverlayParameters(Default::Overlay* overlay, XmlToken& token);
            inline Result handleRectParameters(Default::Rect* rect, XmlToken& token);
            inline Result handleSceneParameters(Default::SceneElement* scene, XmlToken& token);
            inline Result handleSplitParameters(Default::Split* split, XmlToken& token);
            inline Result handleTextParameters(Default::Text* text, XmlToken& token);
            inline Result handleTransformParameters(Default::Transform* transform, XmlToken& token);
    };
}

#endif