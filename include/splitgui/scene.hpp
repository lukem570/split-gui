#ifndef SPLITGUI_SCENE_HPP
#define SPLITGUI_SCENE_HPP

#include "lib.hpp"

#include <splitgui/structs.hpp>
#include <splitgui/graphics.hpp>

#include <vector>

namespace SplitGui {

    class SPLITGUI_EXPORT Node {
        public:

            void setTransform(Transform& transform);
            void setPosition(Vec3& position);
            void setRotation(Vec3& rotation);
            void addChild(Node& child);
            void submitGraphics(Graphics& graphics);

            virtual void submit(int sceneNumber) {};

        protected:
            Graphics*              pGraphics;

            Transform transform;
            std::vector<Node*> children;
    };
    
    class SPLITGUI_EXPORT Mesh : public Node {
        public:

            void submit(int sceneNumber) override;

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

    class SPLITGUI_EXPORT Camera : public Node {
        public:
            void update();
            Mat4 getView();

        protected:

            Vec3 forward, right, up;
    };

    class SPLITGUI_EXPORT Scene {
        public:

        private:
            Node* sceneTree;
    };

}

#endif