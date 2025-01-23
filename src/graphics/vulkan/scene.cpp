#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::instanceScene(Vec2 x1, Vec2 x2) { // need to make the scenes have a description for uploading meshes
        Scene scene;
        scene.viewport.width    = std::abs(x1.x - x2.x);
        scene.viewport.height   = std::abs(x1.y - x2.y);
        scene.viewport.x        = std::min(x1.x,  x2.x);
        scene.viewport.y        = std::min(x1.y,  x2.y);
        scene.cameraFieldOfView = 90;
        scene.cameraPosition    = {0, 0, 0};
        scene.cameraRotation    = {0, 0, 0};

        scenes.push_back(scene);

        createScenesUniforms(); // TODO: fix speed
    }
}