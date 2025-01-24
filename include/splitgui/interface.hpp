#ifndef SPLITGUI_INTERFACE_HPP
#define SPLITGUI_INTERFACE_HPP

#include "lib.h"

#include <splitgui/structs.hpp>
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

    class SPLITGUI_EXPORT InterfaceElement {
        public:

            friend class Interface;

            virtual void instance();

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
                void instance() override;
                void setVertical(bool);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eSplit;
                std::string                    name        = "split";
                const static unsigned int      maxChildren = 2;

            private: // props
                float                          position    = 0.5f; // TODO: get unit operations working
                bool                           isVertical  = false;
        };

        class SPLITGUI_EXPORT List : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eList;
                std::string                    name        = "list";
                const static unsigned int      maxChildren = -1;

            private: // props
                UnitOperation itemWidth;
                bool          isVertical;
        };

        class SPLITGUI_EXPORT Box : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eBox;
                std::string                    name        = "box";
                const static unsigned int      maxChildren = 1;

            private: // props
                std::array<UnitOperation, 2> sizeOverride;
                VerticalAnchor               verticalAnchor;
                HorizontalAnchor             horizontalAnchor;
        };

        class SPLITGUI_EXPORT Overlay : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eOverlay;
                std::string                    name        = "overlay";
                const static unsigned int      maxChildren = -1;

            private: // props
                std::vector<unsigned int>      order;
        };

        class SPLITGUI_EXPORT Mask : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMask;
                std::string                    name        = "mask";
                const static unsigned int      maxChildren = 1;

            private: // props
                std::vector<std::vector<Vec4>> pixels;
        };

        class SPLITGUI_EXPORT Transform : public InterfaceElement {
            public:
                void instance() override;

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
                void instance() override;
                void setColor(HexColor color);

            protected:
                InterfaceElementType           type        = InterfaceElementType::eRect;
                std::string                    name        = "rect";
                const static unsigned int      maxChildren = 0;

            private: // props
                HexColor                       color = 0;
        };

        class SPLITGUI_EXPORT Scene : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eScene;
                std::string                    name        = "scene";
                const static unsigned int      maxChildren = 0; // TODO:

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Text : public InterfaceElement {
            public:
                void instance() override;

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
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMedia;
                std::string                    name        = "media";
                const static unsigned int      maxChildren = 0;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Binding : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eBinding;
                std::string                    name        = "binding";
                const static unsigned int      maxChildren = 0;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Meta : public InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type        = InterfaceElementType::eMeta;
                std::string                    name        = "meta";
                const static unsigned int      maxChildren = 0;

            private: // props
                // TODO:
        };
    }

    class SPLITGUI_EXPORT Interface {
        public:

            friend class Window;

            Interface();
            ~Interface();

            void              parseXml(std::string& data);
            void              setInterfaceElement(InterfaceElement* data);
            InterfaceElement* getInterfaceElement();
            void              submitGraphics(Graphics& graphics);
            void              update();
            void              setViewport(RectObj viewport);

        private:
            InterfaceElement* interfaceElement = nullptr;
            Graphics*         pGraphics        = nullptr;
    };
}
#endif