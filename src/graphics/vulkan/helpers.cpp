#include "vulkan.hpp"

namespace SplitGui {
    inline ResultValue<vk::Bool32> VulkanInterface::checkLayers(const std::vector<const char *> &checkNames, const std::vector<vk::LayerProperties> &layers) {
        SPLITGUI_PROFILE;

        for (const auto &name : checkNames) {
            vk::Bool32 found = VK_FALSE;
            for (const auto &layer : layers) {
                if (!strcmp(name, layer.layerName)) {
                    found = VK_TRUE;
                    break;
                }
            }
            if (!found) {
                std::stringstream info;
                info << "Failed to get layer: ";
                info << name;

                Logger::fatal(info.str());
                return Result::eFailedToGetLayer;
            }
        }
        return VK_TRUE;
    }

    inline vk::Extent2D VulkanInterface::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
        SPLITGUI_PROFILE;

        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            vk::Extent2D actualExtent = {(uint32_t)pWindow->getSize().x, (uint32_t)pWindow->getSize().y};
            actualExtent.width  = (((capabilities.minImageExtent.width) > ((((capabilities.maxImageExtent.width) < (actualExtent.width)) ? (capabilities.maxImageExtent.width) : (actualExtent.width)))) ? (capabilities.minImageExtent.width) : ((((capabilities.maxImageExtent.width) < (actualExtent.width)) ? (capabilities.maxImageExtent.width) : (actualExtent.width))));
            actualExtent.height = (((capabilities.minImageExtent.height) > ((((capabilities.maxImageExtent.height) < (actualExtent.height)) ? (capabilities.maxImageExtent.height) : (actualExtent.height)))) ? (capabilities.minImageExtent.height) : ((((capabilities.maxImageExtent.height) < (actualExtent.height)) ? (capabilities.maxImageExtent.height) : (actualExtent.height))));
            return actualExtent;
        }
    }

    inline vk::SurfaceFormatKHR VulkanInterface::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
        SPLITGUI_PROFILE;

        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == vk::Format::eR8G8B8Snorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    inline vk::PresentModeKHR VulkanInterface::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
        SPLITGUI_PROFILE;

        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                return availablePresentMode;
            }
        }

        return vk::PresentModeKHR::eFifo;
    }

    inline vk::ShaderModule VulkanInterface::createShaderModule(const std::vector<char>& code) {
        SPLITGUI_PROFILE;

        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = code.size();
        createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

        return vk_device.createShaderModule(createInfo);
    }

    inline ResultValue<uint32_t> VulkanInterface::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        SPLITGUI_PROFILE;

        vk::PhysicalDeviceMemoryProperties memProperties = vk_physicalDevice.getMemoryProperties();

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        return Result::eFailedToFindSuitableMemoryType;
    }
}