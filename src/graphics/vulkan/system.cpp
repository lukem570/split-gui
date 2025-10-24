#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::createPhysicalDevice() { 
        SPLITGUI_PROFILE;

        std::vector<vk::PhysicalDevice> physicalDevices = vk_instance.enumeratePhysicalDevices();

        int selection = -1;

        for (unsigned int i = 0; i < physicalDevices.size(); i++) {
            Logutil::info("Possible Device: {}", physicalDevices[i].getProperties().deviceName.data());
        }

        std::vector<int> scores;
        scores.resize(physicalDevices.size());

        for (unsigned int i = 0; i < physicalDevices.size(); i++) {

            if (physicalDevices[i].getProperties().apiVersion < vk::ApiVersion12) {
                scores[i] = -1;
                continue;
            }

            std::string name = physicalDevices[i].getProperties().deviceName.data();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            scores[i] = 0;

            if (name.find("nvidia") != std::string::npos || name.find("geforce") != std::string::npos) {
                scores[i]++;
            }

            if (name.find("amd") != std::string::npos || name.find("radeon") != std::string::npos) {
                scores[i]++;
            }

            if (name.find("graphics") != std::string::npos) {
                scores[i]++;
            }
        }

        for (unsigned int i = 0; i < scores.size(); i++) {
            if (scores[i] == -1) {
                continue;
            }

            if (selection == -1) {
                selection = i;
                continue;
            }

            if (scores[selection] < scores[i]) {
                selection = i;
            }
        }

        if (selection == -1) {
            return Result::eDeviceNotFound;
        }

        vk_physicalDevice = physicalDevices[selection];

        Logutil::info("Created Physical Device");
        Logutil::info("Using: {}", vk_physicalDevice.getProperties().deviceName.data());

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

        Logutil::info("Got Queue Families");

        return Result::eSuccess;
    }

    // or logical device
    inline Result VulkanInterface::createDevice() {
        SPLITGUI_PROFILE;

        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount       = 1;
        float prior                      = 1.0f;
        queueCreateInfo.pQueuePriorities = &prior;

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

        Logutil::info("Created Logical Device");

        return Result::eSuccess;
    }

    inline void VulkanInterface::getQueues() {
        SPLITGUI_PROFILE;

        vk_graphicsQueue = vk_device.getQueue(graphicsQueueFamilyIndex, 0);
        vk_presentQueue = vk_device.getQueue(presentQueueFamilyIndex , 0);

        Logutil::info("Instanced Queue Objects");
    }

    inline Result VulkanInterface::createSurface(SplitGui::Window& window) {
        SPLITGUI_PROFILE;

        ResultValue<vk::SurfaceKHR> surfaceRet = window.createSurface(vk_instance);
        
        TRYD(surfaceRet);

        vk_surface = surfaceRet.value;

        Logutil::info("Created Surface");

        return Result::eSuccess;
    }
}