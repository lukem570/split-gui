#include "../vulkan.hpp"

namespace SplitGui {

    inline void VulkanInterface::createSceneDescriptorSetLayout() {
        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = SceneDescriporBindings::eSceneData;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding modelsLayoutBinding;
        modelsLayoutBinding.binding            = SceneDescriporBindings::eModels;
        modelsLayoutBinding.descriptorCount    = 1;
        modelsLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        modelsLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eVertex;
        modelsLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding textureLayoutBinding;
        textureLayoutBinding.binding            = SceneDescriporBindings::eTexture;
        textureLayoutBinding.descriptorCount    = 1;
        textureLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
        textureLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
        textureLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 3> bindings = { 
            sceneLayoutBinding, 
            modelsLayoutBinding,  
            textureLayoutBinding 
        };

        vk::DescriptorSetLayoutCreateInfo createInfo;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings    = bindings.data();
        
        vk::DescriptorSetLayout descriptorSetLayout = vk_device.createDescriptorSetLayout(createInfo);

        vk_sceneDescriptorSetLayout = descriptorSetLayout;

        SPLITGUI_LOG("Created Scene Descriptor Set Layout");
    }

    inline void VulkanInterface::createSceneDescriptorPool() {
        vk::DescriptorPoolSize scenePoolSize;
        scenePoolSize.type            = vk::DescriptorType::eUniformBuffer;
        scenePoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize modelsPoolSize;
        modelsPoolSize.type            = vk::DescriptorType::eUniformBuffer;
        modelsPoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize texturePoolSize;
        texturePoolSize.type            = vk::DescriptorType::eCombinedImageSampler;
        texturePoolSize.descriptorCount = 1;

        std::array<vk::DescriptorPoolSize, 3> poolSizes = { 
            scenePoolSize, 
            modelsPoolSize, 
            texturePoolSize 
        };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.flags         = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = MAX_SCENES;
        
        vk::DescriptorPool descriptorPool = vk_device.createDescriptorPool(createInfo);

        vk_sceneDescriptorPool = descriptorPool;

        SPLITGUI_LOG("Created Scene Descriptor Pool");
    }

    inline void VulkanInterface::createSceneDescriptorSet(SceneObject& scene) {
        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vk_sceneDescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &vk_sceneDescriptorSetLayout;

        vk::DescriptorSet descriptorSet = vk_device.allocateDescriptorSets(allocInfo).back();

        scene.descriptorSet = descriptorSet;

        SPLITGUI_LOG("Created Scene Descriptor Set");
    }

    inline void VulkanInterface::updateSceneDescriptorSet(SceneObject& scene) {
        vk::DescriptorBufferInfo sceneDataBufferInfo;
        sceneDataBufferInfo.buffer = scene.dataUniformBuffer;
        sceneDataBufferInfo.offset = 0;
        sceneDataBufferInfo.range  = sizeof(SceneObj);

        vk::DescriptorImageInfo textureImageInfo;
        textureImageInfo.sampler     = vk_textureArraySampler;
        textureImageInfo.imageView   = vk_textureArrayImageView;
        textureImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites;

        descriptorWrites[0].dstSet          = scene.descriptorSet;
        descriptorWrites[0].dstBinding      = SceneDescriporBindings::eSceneData;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &sceneDataBufferInfo;

        descriptorWrites[1].dstSet          = scene.descriptorSet;
        descriptorWrites[1].dstBinding      = SceneDescriporBindings::eTexture;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo      = &textureImageInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        SPLITGUI_LOG("Updated Scene Descriptor Sets");
    }
}