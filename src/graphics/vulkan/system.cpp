#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createPhysicalDevice() { 
        std::vector<vk::PhysicalDevice> physicalDevices = vk_instance.enumeratePhysicalDevices();

        int selection = -1;
        
        for (int i = 0; i < physicalDevices.size(); i++) {
            if (physicalDevices[i].getProperties().apiVersion < vk::ApiVersion12) {
                continue;
            }

            selection = i;
            break;
        }

        if (selection == -1) {
            printf("ERROR: No suitable devices found for vulkan\n");
            throw;
        }

        vk_physicalDevice = physicalDevices[selection];

        printf("Using %s\n", vk_physicalDevice.getProperties().deviceName.data());
    }

    inline void VulkanInterface::getQueueFamilies() {
        std::vector<vk::QueueFamilyProperties> queueFamilies = vk_physicalDevice.getQueueFamilyProperties();

        for (int i = 0; i < queueFamilies.size(); i++) {

            vk::Bool32 result = vk_physicalDevice.getSurfaceSupportKHR(i, vk_surface);

            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && graphicsQueueFamilyIndex == -1){
                graphicsQueueFamilyIndex = i;
            }

            if (result && presentQueueFamilyIndex == -1) {
                presentQueueFamilyIndex = i;
            }
            
            if (graphicsQueueFamilyIndex != -1 && presentQueueFamilyIndex != -1) {
                break;
            }
        }

        if (graphicsQueueFamilyIndex == -1) {
            printf("Graphics queue family not found\n");
            throw;
        }

        if (presentQueueFamilyIndex == -1) {
            printf("Present queue family not found\n");
            throw;
        }
    }

    // or logical device
    inline void VulkanInterface::createDevice() {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount       = 1;
        float queuePriority              = 1.0;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        vk::PhysicalDeviceFeatures deviceFeatures;
        deviceFeatures = vk_physicalDevice.getFeatures();

        bool swapchainExtFound = false;

        std::vector<vk::ExtensionProperties> device_extension_return = vk_physicalDevice.enumerateDeviceExtensionProperties();

        for (const auto &extension : device_extension_return) {
            if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, extension.extensionName)) {
                swapchainExtFound = true;
                enabled_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            } else if (!strcmp("VK_KHR_portability_subset", extension.extensionName)) {
                enabled_device_extensions.push_back("VK_KHR_portability_subset");
            }
        }

        if (!swapchainExtFound) {
            printf("Error: could not find swapchain extension\n");
            throw;
        }

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.queueCreateInfoCount    = 1;
        deviceCreateInfo.pQueueCreateInfos       = &queueCreateInfo;
        deviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount   = enabled_device_extensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = enabled_device_extensions.data();

        vk_device = vk_physicalDevice.createDevice(deviceCreateInfo);

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_device);
    }

    inline void VulkanInterface::getQueues() {
        vk_graphicsQueue = vk_device.getQueue(graphicsQueueFamilyIndex, 0);
        vk_presentQueue  = vk_device.getQueue(presentQueueFamilyIndex , 0);
    }

    inline void VulkanInterface::createSurface(SplitGui::Window& window) {
        vk_surface = window.createSurface(vk_instance);
    }
}