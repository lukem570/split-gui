#ifndef SPLITGUI_INTERFACE_HPP
#define SPLITGUI_INTERFACE_HPP

#include "lib.h"

#include <splitgui/structs.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace SplitGui {

    class Window;

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
            virtual void instance();

        protected:
            InterfaceElementType           type     = InterfaceElementType::eRoot;
            std::string                    name     = "root";
            std::vector<InterfaceElement*> children;
            Vec2                           size;
    };

    namespace Default {

        class SPLITGUI_EXPORT Split : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eSplit;
                std::string                    name     = "split";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                UnitOperation position;
                bool          isVertical;
        };

        class SPLITGUI_EXPORT List : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eList;
                std::string                    name     = "list";
                std::vector<InterfaceElement*> children;
                std::array<UnitOperation, 2>   size;

            private: // props
                UnitOperation itemWidth;
                bool          isVertical;
        };

        class SPLITGUI_EXPORT Box : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eBox;
                std::string                    name     = "box";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                std::array<UnitOperation, 2> sizeOverride;
                VerticalAnchor               verticalAnchor;
                HorizontalAnchor             horizontalAnchor;
        };

        class SPLITGUI_EXPORT Overlay : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eOverlay;
                std::string                    name     = "overlay";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                std::vector<unsigned int>      order;
        };

        class SPLITGUI_EXPORT Mask : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eMask;
                std::string                    name     = "mask";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                std::vector<std::vector<Vec4>> pixels;
        };

        class SPLITGUI_EXPORT Transform : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eTransform;
                std::string                    name     = "transform";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                Vec2                           scale;
                Vec2                           position;
                Vec2                           rotation;
        };

        class SPLITGUI_EXPORT Rect : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eRect;
                std::string                    name     = "rect";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                IVec3                          color; //255rgb
        };

        class SPLITGUI_EXPORT Scene : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eScene;
                std::string                    name     = "scene";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Text : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eText;
                std::string                    name     = "text";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                unsigned int                   fontSize;
                std::string                    font;
        };

        class SPLITGUI_EXPORT Media : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eMedia;
                std::string                    name     = "media";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Binding : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eBinding;
                std::string                    name     = "binding";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                // TODO:
        };

        class SPLITGUI_EXPORT Meta : InterfaceElement {
            public:
                void instance() override;

            protected:
                InterfaceElementType           type     = InterfaceElementType::eMeta;
                std::string                    name     = "meta";
                std::vector<InterfaceElement*> children;
                Vec2                           size;

            private: // props
                // TODO:
        };
    }

    class SPLITGUI_EXPORT Interface {
        public:

            friend class Window;

            Interface();
            ~Interface();

            void parseXml(std::string& data);

            void setInterfaceElement(InterfaceElement& data);
            InterfaceElement* getInterfaceElement();

        private:
            InterfaceElement* interfaceElement = nullptr;
    };
}
#endif