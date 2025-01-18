#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::createDescriptorSetLayout() {

        vk::DescriptorSetLayoutBinding uniformLayoutBinding;
        uniformLayoutBinding.binding            = 0;
        uniformLayoutBinding.descriptorCount    = 1;
        uniformLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        uniformLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eVertex;
        uniformLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = 1;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eVertex;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding textureLayoutBinding;
        textureLayoutBinding.binding            = 2;
        textureLayoutBinding.descriptorCount    = 1;
        textureLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        textureLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        textureLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 3> bindings = { uniformLayoutBinding, sceneLayoutBinding, textureLayoutBinding };

        vk::DescriptorSetLayoutCreateInfo createInfo;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings    = bindings.data();
        
        vk_descriptorSetLayout = vk_device.createDescriptorSetLayout(createInfo);
    }

    void VulkanInterface::createDescriptorPool() {
        vk::DescriptorPoolSize uniformPoolSize;
        uniformPoolSize.type            = vk::DescriptorType::eUniformBuffer;
        uniformPoolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        vk::DescriptorPoolSize scenePoolSize;
        scenePoolSize.type            = vk::DescriptorType::eUniformBuffer;
        scenePoolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        vk::DescriptorPoolSize texturePoolSize;
        texturePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        texturePoolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        std::array<vk::DescriptorPoolSize, 3> poolSizes = { uniformPoolSize, scenePoolSize, texturePoolSize };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = MAX_FRAMES_IN_FLIGHT;
        
        vk_descriptorPool = vk_device.createDescriptorPool(createInfo);
    }

    void VulkanInterface::createDescriptorSet() {
        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vk_descriptorPool;
        allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
        allocInfo.pSetLayouts        = &vk_descriptorSetLayout;

        vk_descriptorSets = vk_device.allocateDescriptorSets(allocInfo);
    }
}