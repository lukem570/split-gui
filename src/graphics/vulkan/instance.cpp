#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::instanceVulkan() {
        VkResult err = volkInitialize();
        if (err != VK_SUCCESS) {
            printf("Unable to find the Vulkan runtime on the system\n");
            throw;
        }

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        std::vector<const char *> instanceLayers = {"VK_LAYER_KHRONOS_validation"};

        vk::Bool32 validation_found = VK_FALSE;
        if (vk_validation) {
            std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
            

            validation_found = check_layers(instanceLayers, layers);
            if (validation_found) {
                enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
            } else {
                printf("Error: Could not find validation layers\n");
                throw;
            }
        }

        auto instance_extensions_return = vk::enumerateInstanceExtensionProperties();

        bool surfaceExtFound = false;
        bool platformSurfaceExtFound = false;
        bool portabilityEnumerationActive = false;

        for (const auto &extension : instance_extensions_return) {
            if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                surfaceExtFound = true;
                enabled_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            } else if (!strcmp(VK_KHR_WM_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                platformSurfaceExtFound = true;
                enabled_instance_extensions.push_back(VK_KHR_WM_SURFACE_EXTENSION_NAME);
            }
        }

        if (!surfaceExtFound) {
            printf("Error: failed to find: " VK_KHR_SURFACE_EXTENSION_NAME "\n");
            throw;
        }

        if (!platformSurfaceExtFound) {
            printf("Error: failed to find: " VK_KHR_WM_SURFACE_EXTENSION_NAME "\n");
            throw;
        }

        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName   = "Split-gui";
        appInfo.applicationVersion = 0;
        appInfo.pEngineName        = "Split-gui";
        appInfo.engineVersion      = 0;
        appInfo.apiVersion         = vk::ApiVersion10;

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;

        if (vk_validation) {
            createInfo.enabledLayerCount   = enabled_layers.size();
            createInfo.ppEnabledLayerNames = enabled_layers.data();
        }

        createInfo.setEnabledExtensionCount  (static_cast<uint32_t>(enabled_instance_extensions.size()));
        createInfo.setPpEnabledExtensionNames(                      enabled_instance_extensions.data());

        vk_instance = vk::createInstance(createInfo);

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_instance);
    }
}