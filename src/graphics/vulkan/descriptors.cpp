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

        vk::DescriptorSetLayoutBinding vertexUniformLayoutBinding;
        vertexUniformLayoutBinding.binding            = DescriporBindings::eVertexUniform;
        vertexUniformLayoutBinding.descriptorCount    = 1;
        vertexUniformLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        vertexUniformLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eVertex;
        vertexUniformLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 3> bindings = { sceneLayoutBinding, textureLayoutBinding, vertexUniformLayoutBinding };

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

        vk::DescriptorPoolSize vertexUniformPoolSize;
        vertexUniformPoolSize.type            = vk::DescriptorType::eUniformBuffer;
        vertexUniformPoolSize.descriptorCount = 1;

        std::array<vk::DescriptorPoolSize, 3> poolSizes = { scenePoolSize, texturePoolSize, vertexUniformPoolSize };

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
        vk::DescriptorImageInfo glyphImageInfo;
        glyphImageInfo.sampler     = vk_textGlyphSampler;
        glyphImageInfo.imageView   = vk_textGlyphImageView;
        glyphImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        vk::DescriptorBufferInfo vertexUniformBufferInfo;
        vertexUniformBufferInfo.buffer = vk_vertexUniformBuffer;
        vertexUniformBufferInfo.offset = 0;
        vertexUniformBufferInfo.range  = sizeof(VertexUniformObject);

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites;

        descriptorWrites[0].dstSet          = vk_descriptorSet;
        descriptorWrites[0].dstBinding      = DescriporBindings::eGlyphs;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo      = &glyphImageInfo;

        descriptorWrites[1].dstSet          = vk_descriptorSet;
        descriptorWrites[1].dstBinding      = DescriporBindings::eVertexUniform;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType  = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo     = &vertexUniformBufferInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);
    }
}