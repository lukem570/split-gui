#ifndef SPLITGUI_XML_PARSER_HPP
#define SPLITGUI_XML_PARSER_HPP

#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include <string>
#include <cctype>
#include <unordered_map>

#include "../../path.cpp"

/*
#include "xmlParser.hpp"

namespace SplitGui {

}
*/

namespace SplitGui {

    struct BindPointContainer {
        bool hasOwner          = false;
        InterfaceElement* node = nullptr;
    };

    #define ASSERT_ATTRIBUTE(token) if (token.type != XmlTokenType::eAttribute) { return Result::eInvalidToken; }

    class XmlParser;

    typedef SplitGui::ResultValue<SplitGui::InterfaceElement*> (SplitGui::XmlParser::*HandleTagInterface)();

    class XmlParser {
        public:
               XmlParser();
              ~XmlParser();

[[nodiscard]] ResultValue<InterfaceElement*> parse(const std::string& xmlInput);
        
        private:
              std::unordered_map<std::string, HandleTagInterface> handleTagInterfaces;
              std::unordered_map<std::string, BindPointContainer>  bindings;
              std::string                                         file;
              unsigned int                                        index = 0;

              char currentChar();
              void advance();

[[nodiscard]] ResultValue<XmlToken> nextToken();

[[nodiscard]] ResultValue<InterfaceElement*> parse();

              inline void fillHandleTagInterfaces(std::unordered_map<std::string, HandleTagInterface>& map);

[[nodiscard]] ResultValue<bool> checkClosingTag(std::string name);

[[nodiscard]] ResultValue<InterfaceElement*> handleBindingTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleBoxTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleListTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleSwitchTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleMetaTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleOverlayTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleRectTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleSceneTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleSplitTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleTextTag();
[[nodiscard]] ResultValue<InterfaceElement*> handleTransformTag();

[[nodiscard]] ResultValue<bool> handleDefaultParameters(InterfaceElement* element, XmlToken& token);

[[nodiscard]] inline Result handleBindingParameters(Default::Binding* binding, XmlToken& token);
[[nodiscard]] inline Result handleBoxParameters(Default::Box* box, XmlToken& token);
[[nodiscard]] inline Result handleListParameters(Default::List* list, XmlToken& token);
[[nodiscard]] inline Result handleSwitchParameters(Default::Switch* swi, XmlToken& token);
[[nodiscard]] inline Result handleMetaParameters(Default::Meta* meta, XmlToken& token);
[[nodiscard]] inline Result handleOverlayParameters(Default::Overlay* overlay, XmlToken& token);
[[nodiscard]] inline Result handleRectParameters(Default::Rect* rect, XmlToken& token);
[[nodiscard]] inline Result handleSceneParameters(Default::SceneElement* scene, XmlToken& token);
[[nodiscard]] inline Result handleSplitParameters(Default::Split* split, XmlToken& token);
[[nodiscard]] inline Result handleTextParameters(Default::Text* text, XmlToken& token);
[[nodiscard]] inline Result handleTransformParameters(Default::Transform* transform, XmlToken& token);
    };
}

#endif