#ifndef SPLITGUI_INTERFACE_HPP
#define SPLITGUI_INTERFACE_HPP

#include "lib.hpp"

#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/events.hpp>
#include <splitgui/graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace SplitGui {

    class Window;
    class Interface;

    enum class InterfaceElementType {

        // Concepts
        eSplit,
        eList,
        eBox,
        eOverlay,

        // Modifiers
        eMask,
        eTransform,

        // Objects
        eRect,
        eScene,
        eText,
        eMedia,

        // Misc
        eBinding,
        eMeta,
        eRoot,
        eCustom = 0xFF,
    };

    UnitExpression::BinaryOpType enumerateOperator(char oper) {
        switch (oper) {
            case '+': return UnitExpression::BinaryOpType::eAdd;      break;
            case '-': return UnitExpression::BinaryOpType::eSubtract; break;
            case '*': return UnitExpression::BinaryOpType::eMultiply; break;
            case '/': return UnitExpression::BinaryOpType::eDivide;   break;
            default: throw;
        }
    }

    class SPLITGUI_EXPORT UnitExpressionEvaluator {
        public: 
            ~UnitExpressionEvaluator();

              UnitExpressionValue          evaluate(int maxSize);
              UnitExpressionValue          evaluateExpr(int maxSize, UnitExpression* expression);
[[nodiscard]] ResultValue<UnitExpression*> parse(std::string expression);

        private:
            UnitExpression* expressionTree;
            int             index = 0;

[[nodiscard]] ResultValue<UnitExpressionToken> nextToken(std::string& expression);
              void                             cleanup(UnitExpression* expression);
    };

    class SPLITGUI_EXPORT InterfaceElement {
        public:

            friend class Interface;

[[nodiscard]] virtual Result instance();
              virtual void update();

            void addChild(InterfaceElement* pChild);
            void setSize(IVec2 size);
            void setPosition(IVec2 position);
            void setExtent(RectObj extent);
            void setGraphics(Graphics* pgGraphics);
            void cleanup();

        protected:
            InterfaceElementType           type        = InterfaceElementType::eRoot;
            std::string                    name        = "root";
            const static unsigned int      maxChildren = -1;
            std::vector<InterfaceElement*> children;
            RectObj                        extent;

            Graphics*                      pGraphics   = nullptr;
    };

    namespace Default {

        class SPLITGUI_EXPORT Split : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()     override;

                void   setVertical(bool);
  [[nodiscard]] Result setPosition(std::string);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eSplit;
                std::string                    name        = "split";
                const static unsigned int      maxChildren = 2;

            private: // props
                UnitExpressionEvaluator        position;
                bool                           isVertical  = false;
        };

        class SPLITGUI_EXPORT List : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

                void   setVertical(bool);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eList;
                std::string                    name        = "list";
                const static unsigned int      maxChildren = -1;

            private: // props
                bool           isVertical;
        };

        class SPLITGUI_EXPORT Box : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eBox;
                std::string                    name        = "box";
                const static unsigned int      maxChildren = 1;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Overlay : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eOverlay;
                std::string                    name        = "overlay";
                const static unsigned int      maxChildren = -1;

            private: // props
                std::vector<unsigned int>      order;
        };

        class SPLITGUI_EXPORT Mask : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMask;
                std::string                    name        = "mask";
                const static unsigned int      maxChildren = 1;

            private: // props
                std::vector<std::vector<Vec4>> pixels;
        };

        class SPLITGUI_EXPORT Transform : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eTransform;
                std::string                    name        = "transform";
                const static unsigned int      maxChildren = 1;    

            private: // props
                Vec2                           scale;
                Vec2                           position;
                Vec2                           rotation;
        };

        class SPLITGUI_EXPORT Rect : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

                void setColor(HexColor color);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eRect;
                std::string                    name        = "rect";
                const static unsigned int      maxChildren = 0;

            private: // props
                HexColor                       color = 0;
                
                // state
                RectRef                        graphicsRectRef;
        };

        class SPLITGUI_EXPORT SceneElement : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

                void setSceneNumber(unsigned int sceneNumber);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eScene;
                std::string                    name        = "scene";
                const static unsigned int      maxChildren = 0; // TODO:

            private: // props
                unsigned int                   number = 0;
                // TODO:

                // state
                unsigned int                   graphicsSceneRef;
        };

        class SPLITGUI_EXPORT Text : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eText;
                std::string                    name        = "text";
                const static unsigned int      maxChildren = 1;

            private: // props
                unsigned int                   fontSize;
                std::string                    font;
        };

        class SPLITGUI_EXPORT Media : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMedia;
                std::string                    name        = "media";
                const static unsigned int      maxChildren = 0;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Binding : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

                void   setAlias(std::string& inAlias);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eBinding;
                std::string                    name        = "binding";
                const static unsigned int      maxChildren = 0;

            private: // props
                std::string                    alias;
        };

        class SPLITGUI_EXPORT Meta : public InterfaceElement {
            public:
  [[nodiscard]] Result instance() override;
                void   update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMeta;
                std::string                    name        = "meta";
                const static unsigned int      maxChildren = 0;

            private: // props
                std::string                    version;
        };
    }

    class SPLITGUI_EXPORT Interface {
        public:

            friend class Window;

            Interface();
            ~Interface();

[[nodiscard]] Result            parseXml(std::string& data);
              void              setInterfaceElement(InterfaceElement* data);
              InterfaceElement* getInterfaceElement();
              void              submitGraphics(Graphics& graphics);
              void              update();
              void              instance();
              void              setViewport(RectObj viewport);
              void              attachEventHandler(EventHandler& handler);

        private:
            InterfaceElement* interfaceElement = nullptr;
            Graphics*         pGraphics        = nullptr;
            EventHandler*     pEventHandler    = nullptr;
    };
}
#endif