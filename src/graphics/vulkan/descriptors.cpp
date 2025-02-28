#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::createDescriptorSetLayout() {

        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = DescriporBindings::eSceneData;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding glyphLayoutBinding;
        glyphLayoutBinding.binding            = DescriporBindings::eGlyphs;
        glyphLayoutBinding.descriptorCount    = 1;
        glyphLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        glyphLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        glyphLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding vertexUniformLayoutBinding;
        vertexUniformLayoutBinding.binding            = DescriporBindings::eVertexUniform;
        vertexUniformLayoutBinding.descriptorCount    = 1;
        vertexUniformLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        vertexUniformLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eVertex;
        vertexUniformLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding textureLayoutBinding;
        textureLayoutBinding.binding            = DescriporBindings::eTexture;
        textureLayoutBinding.descriptorCount    = 1;
        textureLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        textureLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        textureLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 4> bindings = { sceneLayoutBinding, glyphLayoutBinding, vertexUniformLayoutBinding, textureLayoutBinding };

        vk::DescriptorSetLayoutCreateInfo createInfo;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings    = bindings.data();
        
        vk_descriptorSetLayout = vk_device.createDescriptorSetLayout(createInfo);

        SPLITGUI_LOG("Created Descriptor Set Layout");
    }

    void VulkanInterface::createDescriptorPool() {
        vk::DescriptorPoolSize scenePoolSize;
        scenePoolSize.type            = vk::DescriptorType::eUniformBuffer;
        scenePoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize glyphPoolSize;
        glyphPoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        glyphPoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize vertexUniformPoolSize;
        vertexUniformPoolSize.type            = vk::DescriptorType::eUniformBuffer;
        vertexUniformPoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize texturePoolSize;
        texturePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        texturePoolSize.descriptorCount = 1;

        std::array<vk::DescriptorPoolSize, 4> poolSizes = { scenePoolSize, glyphPoolSize, vertexUniformPoolSize, texturePoolSize };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.flags         = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = 1;
        
        vk_descriptorPool = vk_device.createDescriptorPool(createInfo);

        SPLITGUI_LOG("Created Descriptor Pool");
    }

    void VulkanInterface::createDescriptorSet() {
        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vk_descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &vk_descriptorSetLayout;

        vk_descriptorSet = vk_device.allocateDescriptorSets(allocInfo).back();

        SPLITGUI_LOG("Created Descriptor Set");
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

        vk::DescriptorImageInfo textureImageInfo;
        textureImageInfo.sampler     = vk_textureArraySampler;
        textureImageInfo.imageView   = vk_textureArrayImageView;
        textureImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        std::array<vk::WriteDescriptorSet, 3> descriptorWrites;

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

        descriptorWrites[2].dstSet          = vk_descriptorSet;
        descriptorWrites[2].dstBinding      = DescriporBindings::eTexture;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pImageInfo     = &textureImageInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        SPLITGUI_LOG("Updated Descriptor Sets");
    }
}