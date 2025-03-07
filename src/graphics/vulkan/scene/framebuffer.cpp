#include "../vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createSceneFramebuffers(SceneObject& scene) {
        std::vector<vk::Framebuffer> framebuffers;

        framebuffers.resize(vk_swapchainImageViews.size());

        std::array<vk::ImageView, 2> attachments = {
            scene.outputImageView,
            scene.depthImageView
        };

        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++) {

            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.renderPass      = vk_renderpass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments    = attachments.data();
            framebufferInfo.width           = scene.sceneSize.x;
            framebufferInfo.height          = scene.sceneSize.y;
            framebufferInfo.layers          = 1;

            framebuffers[i] = vk_device.createFramebuffer(framebufferInfo);
        }

        scene.framebuffers = framebuffers;

        SPLITGUI_LOG("Created Scene Framebuffers");
    }
}