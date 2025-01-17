#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::createDescriptorSetLayout() {

        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = 0;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eGeometry;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding textureLayoutBinding;
        textureLayoutBinding.binding            = 1;
        textureLayoutBinding.descriptorCount    = 1;
        textureLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        textureLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        textureLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { sceneLayoutBinding, textureLayoutBinding };

        vk::DescriptorSetLayoutCreateInfo createInfo;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings    = bindings.data();
        
        vk_descriptorSetLayout = vk_device.createDescriptorSetLayout(createInfo);
    }

    void VulkanInterface::createDescriptorPool() {
        vk::DescriptorPoolSize scenePoolSize;
        scenePoolSize.type            = vk::DescriptorType::eUniformBuffer;
        scenePoolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        vk::DescriptorPoolSize texturePoolSize;
        texturePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        texturePoolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        std::array<vk::DescriptorPoolSize, 2> poolSizes = { scenePoolSize, texturePoolSize };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = MAX_FRAMES_IN_FLIGHT;
        
        vk_descriptorPool = vk_device.createDescriptorPool(createInfo);
    }

    void VulkanInterface::createDescriptorSet() {

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vk::DescriptorImageInfo imageInfo;
            imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            //imageInfo.imageView   = 
        }

        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vk_descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &vk_descriptorSetLayout;

        //vk_descriptorSet = vk_device.allocateDescriptorSets(allocInfo).front();
    }
}