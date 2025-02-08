#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createSyncObj() {
        vk::SemaphoreCreateInfo semaphoreInfo;
        vk::FenceCreateInfo fenceInfo;
        fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

        vk_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        vk_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        vk_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk_imageAvailableSemaphores[i] = vk_device.createSemaphore(semaphoreInfo);
            vk_renderFinishedSemaphores[i] = vk_device.createSemaphore(semaphoreInfo);
            vk_inFlightFences[i]           = vk_device.createFence(fenceInfo);
        }

        SPLITGUI_LOG("Created Sync Objects");
    }
}