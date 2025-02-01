#ifndef SPLITGUI_XML_PARSER_HPP
#define SPLITGUI_XML_PARSER_HPP

#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include <string>
#include <cctype>

/*
#include "xmlParser.hpp"

namespace SplitGui {

}
*/

namespace SplitGui {

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

            ResultValue<InterfaceElement*> parse(int depth);

            inline Result handleSplitParameters(Default::Split* split, XmlToken& token);
            inline Result handleListParameters(Default::List* split, XmlToken& token);
            inline Result handleBoxParameters(Default::Box* split, XmlToken& token);
            inline Result handleOverlayParameters(Default::Overlay* split, XmlToken& token);
            inline Result handleMaskParameters(Default::Mask* split, XmlToken& token);
            inline Result handleTransformParameters(Default::Transform* split, XmlToken& token);
            inline Result handleRectParameters(Default::Rect* rect, XmlToken& token);
            inline Result handleSceneParameters(Default::SceneElement* scene, XmlToken& token);
            inline Result handleTextParameters(Default::Text* scene, XmlToken& token);
            inline Result handleMediaParameters(Default::Media* scene, XmlToken& token);
            inline Result handleBindingParameters(Default::Binding* scene, XmlToken& token);
            inline Result handleMetaParameters(Default::Meta* split, XmlToken& token);

    };
}

#endif