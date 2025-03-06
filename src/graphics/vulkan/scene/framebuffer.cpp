#include "../vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createSceneFramebuffers() {
        std::vector<vk::Framebuffer> framebuffers;

        framebuffers.resize(vk_swapchainImageViews.size());

        std::array<vk::ImageView, 2> attachments = {
            vk_sceneOutputImageViews.back(),
            vk_sceneDepthImageViews.back()
        };

        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++) {

            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.renderPass      = vk_renderpass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments    = attachments.data();
            framebufferInfo.width           = vk_swapchainExtent.width;
            framebufferInfo.height          = vk_swapchainExtent.height;
            framebufferInfo.layers          = 1;

            framebuffers[i] = vk_device.createFramebuffer(framebufferInfo);
        }

        vk_sceneFrameBuffers.push_back(framebuffers);

        SPLITGUI_LOG("Created Scene Framebuffers");
    }
}