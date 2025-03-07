#ifndef SPLITGUI_SCENE_HPP
#define SPLITGUI_SCENE_HPP

#include "lib.hpp"

#include <splitgui/structs.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/result.hpp>

#include <vector>

namespace SplitGui {

    class SPLITGUI_EXPORT Node {
        public:

            void      setTransform(Transform& transform);
            void      setPosition(Vec3 position);
            void      setRotation(Vec3 rotation);
            void      addChild(Node& child);
            void      submitGraphics(Graphics& graphics);

[[nodiscard]] virtual Result submit(SceneRef& ref, int flags = VertexFlagsBits::eNone) { return Result::eSuccess; };

        protected:
            Graphics*              pGraphics;

            Transform transform;
            std::vector<Node*> children;
    };
    
    class SPLITGUI_EXPORT Mesh : public Node {
        public:

[[nodiscard]] Result submit(SceneRef& ref, int flags = VertexFlagsBits::eNone) override;

        protected:

            std::vector<Vertex>   vertices;
            std::vector<uint16_t> indices;
    };

    class SPLITGUI_EXPORT Cube : public Mesh {
        public:
            Cube();

            void setSize(float size);
            void setColor(HexColor color);
            void generate();
            
            protected:
            
            float size = 1;
            HexColor color;
        };
        
        class SPLITGUI_EXPORT Grid : public Mesh {
            public:
            Grid();
            
[[nodiscard]] Result submit(SceneRef& ref, int flags = VertexFlagsBits::eWorldSpace) override;

            void setColor(HexColor color);
            void generate();
            
        protected:

            HexColor color = 0;
    };

    class SPLITGUI_EXPORT Camera : public Node {
        public:
[[nodiscard]] Result update(SceneRef& ref);
              Mat4   getView();

        protected:

            Mat4 view;
    };

    class SPLITGUI_EXPORT Scene {
        public:

        private:
            Node* sceneTree;
    };

}

#endif