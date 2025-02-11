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

    void Node::setPosition(Vec3& position) {
        transform.position = position;
    }

    void Node::setRotation(Vec3& rotation) {
        transform.rotation = rotation;
    }

}