#ifndef SPLITGUI_SCENE_HPP
#define SPLITGUI_SCENE_HPP

#include "lib.hpp"

#include <splitgui/structs.hpp>

namespace SplitGui {

    class Node {
        Transform transform;
        std::vector<Node*> children;
    };
    
    class Mesh : Node {

    };

    class Cube : Mesh {

    };

    class Prism : Mesh {

    };

    class Cylinder : Mesh {

    };

    class Hedron : Mesh {

    };

    class Sphere : Mesh {

    };

    class Pyramid : Mesh {

    };

    class Cone : Mesh {

    };

    class Scene {
        public:

        private:
            Node* sceneTree;
    };

}

#endif