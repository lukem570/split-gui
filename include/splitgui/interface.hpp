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

#define DEPTH_PLANE 255

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
            UnitExpression* expressionTree = nullptr;
            unsigned int    index = 0;

[[nodiscard]] ResultValue<UnitExpressionToken> nextToken(std::string& expression);
              void                             cleanup(UnitExpression* expression);
    };

    class SPLITGUI_EXPORT InterfaceElement {
        public:

            virtual ~InterfaceElement() = default;

            friend class Interface;

[[nodiscard]] virtual Result instance();
[[nodiscard]] virtual Result update();
[[nodiscard]] virtual Result submit();

            void addChild(InterfaceElement* pChild);
            void setSize(IVec2 size);
            void setPosition(IVec2 position);
            void setExtent(RectObj extent);
            void setGraphics(Graphics* pgGraphics);
            void setRef(std::string string);
            void setDepth(int depth);
            void cleanup();

            virtual std::vector<InterfaceElement*> searchByReference(std::string reference);

            RectObj getExtent();

        protected:
            InterfaceElementType           type        = InterfaceElementType::eRoot;
            std::string                    name        = "root";
            const static unsigned int      maxChildren = -1;
            std::vector<InterfaceElement*> children;
            RectObj                        extent;
            std::string                    ref         = "";
            int                            depth       = 0;
            

            Graphics*                      pGraphics   = nullptr;
    };

    namespace Default {

        class SPLITGUI_EXPORT Split : public InterfaceElement {
            public:
                      ~Split()    override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

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
                      ~List()     override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

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
                      ~Box()      override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eBox;
                std::string                    name        = "box";
                const static unsigned int      maxChildren = 1;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Overlay : public InterfaceElement {
            public:
                      ~Overlay()    override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eOverlay;
                std::string                    name        = "overlay";
                const static unsigned int      maxChildren = -1;

            private: // props
                std::vector<unsigned int>      order;
        };

        class SPLITGUI_EXPORT Transform : public InterfaceElement {
            public:
                      ~Transform() override = default;

  [[nodiscard]] Result instance()  override;
  [[nodiscard]] Result update()   override;

  [[nodiscard]] Result setScale(std::string);
  [[nodiscard]] Result setPosition(std::string);
                void   setOrigin(bool isRelative);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eTransform;
                std::string                    name        = "transform";
                const static unsigned int      maxChildren = 1;    

            private: // props
                UnitExpressionEvaluator        position;
                UnitExpressionEvaluator        scale;
                bool                           isRelative = true;
        };

        class SPLITGUI_EXPORT Rect : public InterfaceElement {
            public:
                      ~Rect()     override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;
  [[nodiscard]] Result submit()   override;

                void   setColor(HexColor);
  [[nodiscard]] Result setColor(std::string);
                void   setFlags(VertexFlags flags);
                void   setTextureIndex(int idx);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eRect;
                std::string                    name        = "rect";
                const static unsigned int      maxChildren = 0;

            private: // props
                UnitExpressionEvaluator        colorStatement;
                HexColor                       color = 0;
                VertexFlags                    flags = 0;
                int                            textureIndex = 0;
                
                // state
                RectRef                        graphicsRectRef;
        };

        class SPLITGUI_EXPORT SceneElement : public InterfaceElement {
            public:
                      ~SceneElement() override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

                SceneRef getSceneRef();

            protected:
                InterfaceElementType           type        = InterfaceElementType::eScene;
                std::string                    name        = "scene";
                const static unsigned int      maxChildren = 0; // TODO:

            private: // props

                // state
                SceneRef                       graphicsSceneRef;
        };

        class SPLITGUI_EXPORT Text : public InterfaceElement {
            public:
                      ~Text()     override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

                void   setText(std::string value);
                void   setColor(HexColor);
  [[nodiscard]] Result setColor(std::string);
                void   setSize(unsigned int size);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eText;
                std::string                    name        = "text";
                const static unsigned int      maxChildren = 1;

            private: // props
                unsigned int                   fontSize = 12;
                std::string                    font; // TODO:
                std::string                    value;
                UnitExpressionEvaluator        colorStatement;
                HexColor                       color = 0;

                // state
                TextRef                        textRef;
        };

        class SPLITGUI_EXPORT Binding : public InterfaceElement {
            public:
                      ~Binding()  override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

                void   setAlias(std::string& inAlias);
                void   setSource(std::string& inSource);

                std::string   getAlias();
                std::string   getSource();

            protected:
                InterfaceElementType           type        = InterfaceElementType::eBinding;
                std::string                    name        = "binding";
                const static unsigned int      maxChildren = 0;

            private: // props
                std::string                    alias;
                std::string                    source;
        };

        class SPLITGUI_EXPORT Meta : public InterfaceElement {
            public:
                      ~Meta()     override = default;

  [[nodiscard]] Result instance() override;
  [[nodiscard]] Result update()   override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMeta;
                std::string                    name        = "meta";
                const static unsigned int      maxChildren = 0;

            private: // props
                std::string                    version;
        };

        class SPLITGUI_EXPORT BindPoint : public InterfaceElement {
            public:
                      ~BindPoint() override;

  [[nodiscard]] Result instance()  override;
  [[nodiscard]] Result update()   override;

                void   setBindPoint(InterfaceElement*);
                void   setOwnership(bool);

                std::vector<InterfaceElement*> searchByReference(std::string reference) override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMeta;
                std::string                    name        = "meta";
                const static unsigned int      maxChildren = 0;

            private: 
                InterfaceElement*              root = nullptr;
                bool                           owner = false;
        };
    }

    class SPLITGUI_EXPORT Interface {
        public:

            friend class Window;

            Interface();
            ~Interface();

[[nodiscard]] Result                         parseXml(std::string& data);
              void                           setInterfaceElement(InterfaceElement* data);
              InterfaceElement*              getInterfaceElement();
              void                           submitGraphics(Graphics& graphics);
[[nodiscard]] Result                         update();
[[nodiscard]] Result                         instance();
              void                           setViewport(RectObj viewport);
              void                           attachEventHandler(EventHandler& handler);
              std::vector<InterfaceElement*> searchByReference(std::string reference);

        private:
            InterfaceElement* interfaceElement = nullptr;
            Graphics*         pGraphics        = nullptr;
            EventHandler*     pEventHandler    = nullptr;
    };
}
#endif