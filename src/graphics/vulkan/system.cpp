#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::createPhysicalDevice() { 
        SPLITGUI_PROFILE;

        std::vector<vk::PhysicalDevice> physicalDevices = vk_instance.enumeratePhysicalDevices();

        int selection = -1;

        for (unsigned int i = 0; i < physicalDevices.size(); i++) {
            std::string deviceName = physicalDevices[i].getProperties().deviceName.data();
            std::stringstream info;
            info << "Possible Device: " << deviceName;
            Logger::info(info.str());
        }

        for (unsigned int i = 0; i < physicalDevices.size(); i++) {
            if (physicalDevices[i].getProperties().apiVersion < vk::ApiVersion12) {
                continue;
            }

            selection = i;
        }

        if (selection == -1) {
            return Result::eDeviceNotFound;
        }

        vk_physicalDevice = physicalDevices[selection];

        Logger::info("Created Physical Device");

        std::string deviceName = vk_physicalDevice.getProperties().deviceName.data();
        std::stringstream info;
        info << "Using: " << deviceName;
        Logger::info(info.str());

        return Result::eSuccess;
    }

    inline Result VulkanInterface::getQueueFamilies() {
        SPLITGUI_PROFILE;

        std::vector<vk::QueueFamilyProperties> queueFamilies = vk_physicalDevice.getQueueFamilyProperties();

        for (unsigned int i = 0; i < queueFamilies.size(); i++) {

            vk::Bool32 result = vk_physicalDevice.getSurfaceSupportKHR(i, vk_surface);

            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && graphicsQueueFamilyIndex == (unsigned int)-1){
                graphicsQueueFamilyIndex = i;
            }

            if (result && presentQueueFamilyIndex == (unsigned int)-1) {
                presentQueueFamilyIndex = i;
            }
            
            if (graphicsQueueFamilyIndex != (unsigned int)-1 && presentQueueFamilyIndex != (unsigned int)-1) {
                break;
            }
        }

        if (graphicsQueueFamilyIndex == (unsigned int)-1) {
            return Result::eFailedToFindQueueFamily;
        }

        if (presentQueueFamilyIndex == (unsigned int)-1) {
            return Result::eFailedToFindQueueFamily;
        }

        Logger::info("Got Queue Families");

        return Result::eSuccess;
    }

    // or logical device
    inline Result VulkanInterface::createDevice() {
        SPLITGUI_PROFILE;

        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount       = 1;
        float queuePriority              = 1.0;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        vk::PhysicalDeviceFeatures deviceFeatures;
        deviceFeatures = vk_physicalDevice.getFeatures();

        bool swapchainExtFound = false;

        std::vector<vk::ExtensionProperties> deviceExtensionReturn = vk_physicalDevice.enumerateDeviceExtensionProperties();

        for (const auto &extension : deviceExtensionReturn) {
            if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, extension.extensionName)) {
                swapchainExtFound = true;
                enabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            } else if (!strcmp("VK_KHR_portability_subset", extension.extensionName)) {
                enabledDeviceExtensions.push_back("VK_KHR_portability_subset");
            }
        }

        if (!swapchainExtFound) {
            return Result::eFailedToFindSwapchainExtension;
        }

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.queueCreateInfoCount    = 1;
        deviceCreateInfo.pQueueCreateInfos       = &queueCreateInfo;
        deviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount   = enabledDeviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();

        vk_device = vk_physicalDevice.createDevice(deviceCreateInfo);

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_device);

        Logger::info("Created Logical Device");

        return Result::eSuccess;
    }

    inline void VulkanInterface::getQueues() {
        SPLITGUI_PROFILE;

        vk_graphicsQueue = vk_device.getQueue(graphicsQueueFamilyIndex, 0);
        vk_presentQueue  = vk_device.getQueue(presentQueueFamilyIndex , 0);

        Logger::info("Instanced Queue Objects");
    }

    inline Result VulkanInterface::createSurface(SplitGui::Window& window) {
        SPLITGUI_PROFILE;

        ResultValue<vk::SurfaceKHR> surfaceRet = window.createSurface(vk_instance);
        
        TRYD(surfaceRet);

        vk_surface = surfaceRet.value;

        Logger::info("Created Surface");

        return Result::eSuccess;
    }
}