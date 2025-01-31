#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::instanceVulkan() {
        VkResult err = volkInitialize();
        if (err != VK_SUCCESS) {
            return Result::eVulkanNotFound;
        }

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        std::vector<const char *> instanceLayers = {"VK_LAYER_KHRONOS_validation"};

        ResultValue<vk::Bool32> validationFound = VK_FALSE;
        if (vk_validation) {
            std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
            

            validationFound = checkLayers(instanceLayers, layers);

            TRYD(validationFound);

            if (validationFound.value) {
                enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
            } else {
                return Result::eFailedToGetLayer;
            }
        }

        auto instanceExtensionsReturn = vk::enumerateInstanceExtensionProperties();

        bool surfaceExtFound = false;
        bool platformSurfaceExtFound = false;

        for (const auto &extension : instanceExtensionsReturn) {
            if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                surfaceExtFound = true;
                enabledInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            } else if (!strcmp(VK_KHR_WM_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                platformSurfaceExtFound = true;
                enabledInstanceExtensions.push_back(VK_KHR_WM_SURFACE_EXTENSION_NAME);
            }
        }

        if (!surfaceExtFound) {
            return Result::eFailedToFindSurfaceExtension;
        }

        if (!platformSurfaceExtFound) {
            return Result::eFailedToFindSurfaceExtension;
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
            createInfo.enabledLayerCount   = enabledLayers.size();
            createInfo.ppEnabledLayerNames = enabledLayers.data();
        }

        createInfo.setEnabledExtensionCount  (static_cast<uint32_t>(enabledInstanceExtensions.size()));
        createInfo.setPpEnabledExtensionNames(                      enabledInstanceExtensions.data());

        vk_instance = vk::createInstance(createInfo);

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_instance);

        return Result::eSuccess;
    }
}