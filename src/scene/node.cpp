#include <splitgui/scene.hpp>

namespace SplitGui {
    void Node::setTransform(Transform& transformIn) {
        transform = transformIn;
    }

    void Node::addChild(Node& child) {
        children.push_back(&child);
    }

    void Node::submitGraphics(Graphics& graphics) {
        pGraphics = &graphics;
    }
}