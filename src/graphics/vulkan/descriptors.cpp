#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::createDescriptorSetLayout() {

        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = DescriporBindings::eSceneData;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding textureLayoutBinding;
        textureLayoutBinding.binding            = DescriporBindings::eGlyphs;
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
        scenePoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize texturePoolSize;
        texturePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        texturePoolSize.descriptorCount = 1;

        std::array<vk::DescriptorPoolSize, 2> poolSizes = { scenePoolSize, texturePoolSize };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = 1;
        
        vk_descriptorPool = vk_device.createDescriptorPool(createInfo);
    }

    void VulkanInterface::createDescriptorSet() {
        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vk_descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &vk_descriptorSetLayout;

        vk_descriptorSet = vk_device.allocateDescriptorSets(allocInfo).back();
    }

    inline void VulkanInterface::updateDescriptorSets() {
        vk::DescriptorImageInfo descriptorImageInfo;
        descriptorImageInfo.sampler     = vk_textGlyphSampler;
        descriptorImageInfo.imageView   = vk_textGlyphImageView;
        descriptorImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        std::array<vk::WriteDescriptorSet, 1> descriptorWrites;

        descriptorWrites[0].dstSet          = vk_descriptorSet;
        descriptorWrites[0].dstBinding      = DescriporBindings::eGlyphs;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo      = &descriptorImageInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);
    }
}