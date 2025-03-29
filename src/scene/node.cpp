#include <splitgui/scene.hpp>

namespace SplitGui {
    void Node::setTransform(Transform& transformIn) {
        SPLITGUI_PROFILE;

        transform = transformIn;
    }

    void Node::addChild(Node& child) {
        SPLITGUI_PROFILE;

        children.push_back(&child);
    }

    void Node::submitGraphics(Graphics& graphics) {
        SPLITGUI_PROFILE;

        pGraphics = &graphics;
    }

    void Node::setPosition(Vec3 position) {
        SPLITGUI_PROFILE;

        transform.position = position;
    }

    void Node::setRotation(Vec3 rotation) {
        SPLITGUI_PROFILE;

        transform.rotation = rotation;
    }

}