#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::createDescriptorSetLayout() {

        vk::DescriptorSetLayoutBinding glyphLayoutBinding;
        glyphLayoutBinding.binding            = DescriporBindings::eGlyphs;
        glyphLayoutBinding.descriptorCount    = 1;
        glyphLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        glyphLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        glyphLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = DescriporBindings::eScenes;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding textureLayoutBinding;
        textureLayoutBinding.binding            = DescriporBindings::eTexture;
        textureLayoutBinding.descriptorCount    = 1;
        textureLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        textureLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        textureLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 3> bindings = { 
            glyphLayoutBinding,
            sceneLayoutBinding,
            textureLayoutBinding,
        };

        vk::DescriptorSetLayoutCreateInfo createInfo;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings    = bindings.data();
        
        vk_descriptorSetLayout = vk_device.createDescriptorSetLayout(createInfo);

        Logger::info("Created Descriptor Set Layout");
    }

    void VulkanInterface::createDescriptorPool() {
        
        vk::DescriptorPoolSize glyphPoolSize;
        glyphPoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        glyphPoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize scenePoolSize;
        scenePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        scenePoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize texturePoolSize;
        texturePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        texturePoolSize.descriptorCount = 1;

        std::array<vk::DescriptorPoolSize, 3> poolSizes = { 
            glyphPoolSize,
            scenePoolSize,
            texturePoolSize,
        };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.flags         = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = 1;
        
        vk_descriptorPool = vk_device.createDescriptorPool(createInfo);

        Logger::info("Created Descriptor Pool");
    }

    void VulkanInterface::createDescriptorSet() {
        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vk_descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &vk_descriptorSetLayout;

        vk_descriptorSet = vk_device.allocateDescriptorSets(allocInfo).back();

        Logger::info("Created Descriptor Set");
    }

    inline void VulkanInterface::updateDescriptorSets() {
        vk::DescriptorImageInfo glyphImageInfo;
        glyphImageInfo.sampler     = vk_textGlyphSampler;
        glyphImageInfo.imageView   = vk_textGlyphImageView;
        glyphImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        vk::DescriptorImageInfo scenesImageInfo;
        scenesImageInfo.sampler     = vk_scenesImageArraySampler;
        scenesImageInfo.imageView   = vk_scenesImageArrayImageView;
        scenesImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

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
        descriptorWrites[1].dstBinding      = DescriporBindings::eScenes;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo      = &scenesImageInfo;

        descriptorWrites[2].dstSet          = vk_descriptorSet;
        descriptorWrites[2].dstBinding      = DescriporBindings::eTexture;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pImageInfo      = &textureImageInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        Logger::info("Updated Descriptor Sets");
    }
}