#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::instanceScene(Vec2 x1, Vec2 x2) { // need to make the scenes have a description for uploading meshes
        RectObj scene;
        scene.width  = std::abs(x1.x - x2.x);
        scene.height = std::abs(x1.y - x2.y);
        scene.x      = std::min(x1.x,  x2.x);
        scene.y      = std::min(x1.y,  x2.y);

        scenes.push_back(scene);
    }
}