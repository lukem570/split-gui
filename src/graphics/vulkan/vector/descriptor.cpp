#include "../vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createVectorEngineDescriptorSetLayout() {
        SPLITGUI_PROFILE;

        vk::DescriptorSetLayoutBinding sceneLayoutBinding;
        sceneLayoutBinding.binding            = VectorEngineDescriporBindings::eSceneData;
        sceneLayoutBinding.descriptorCount    = 1;
        sceneLayoutBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
        sceneLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eCompute;
        sceneLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding edgesInLayoutBinding;
        edgesInLayoutBinding.binding            = VectorEngineDescriporBindings::eEdgesIn;
        edgesInLayoutBinding.descriptorCount    = 1;
        edgesInLayoutBinding.descriptorType     = vk::DescriptorType::eStorageBuffer;
        edgesInLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eCompute;
        edgesInLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding edgesOutLayoutBinding;
        edgesOutLayoutBinding.binding            = VectorEngineDescriporBindings::eEdgesOut;
        edgesOutLayoutBinding.descriptorCount    = 1;
        edgesOutLayoutBinding.descriptorType     = vk::DescriptorType::eStorageBuffer;
        edgesOutLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eCompute;
        edgesOutLayoutBinding.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutBinding outputImageLayoutBinding;
        outputImageLayoutBinding.binding            = VectorEngineDescriporBindings::eOutputImage;
        outputImageLayoutBinding.descriptorCount    = 1;
        outputImageLayoutBinding.descriptorType     = vk::DescriptorType::eStorageImage;
        outputImageLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eCompute;
        outputImageLayoutBinding.pImmutableSamplers = nullptr;

        std::array<vk::DescriptorSetLayoutBinding, 4> bindings = { 
            sceneLayoutBinding, 
            edgesInLayoutBinding,  
            edgesOutLayoutBinding,
            outputImageLayoutBinding
        };

        vk::DescriptorSetLayoutCreateInfo createInfo;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings    = bindings.data();
        
        vk::DescriptorSetLayout descriptorSetLayout = vk_device.createDescriptorSetLayout(createInfo);

        vk_vectorEngineDescriptorSetLayout = descriptorSetLayout;

        Logger::info("Created Vector Engine Descriptor Set Layout");
    }

    inline void VulkanInterface::createVectorEngineDescriptorPool(VectorEngineObject& vEngine) {
        SPLITGUI_PROFILE;

        vk::DescriptorPoolSize scenePoolSize;
        scenePoolSize.type            = vk::DescriptorType::eUniformBuffer;
        scenePoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize edgesInPoolSize;
        edgesInPoolSize.type            = vk::DescriptorType::eStorageBuffer;
        edgesInPoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize edgesOutPoolSize;
        edgesOutPoolSize.type            = vk::DescriptorType::eStorageBuffer;
        edgesOutPoolSize.descriptorCount = 1;

        vk::DescriptorPoolSize outputImagePoolSize;
        outputImagePoolSize.type            = vk::DescriptorType::eStorageImage;
        outputImagePoolSize.descriptorCount = 1;

        std::array<vk::DescriptorPoolSize, 4> poolSizes = { 
            scenePoolSize, 
            edgesInPoolSize, 
            edgesOutPoolSize,
            outputImagePoolSize
        };

        vk::DescriptorPoolCreateInfo createInfo;
        createInfo.flags         = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        createInfo.poolSizeCount = poolSizes.size();
        createInfo.pPoolSizes    = poolSizes.data();
        createInfo.maxSets       = 1;
        
        vk::DescriptorPool descriptorPool = vk_device.createDescriptorPool(createInfo);

        vEngine.descriptorPool = descriptorPool;

        Logger::info("Created Vector Engine Descriptor Pool");
    }

    inline void VulkanInterface::createVectorEngineDescriptorSet(VectorEngineObject& vEngine) {
        SPLITGUI_PROFILE;

        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.descriptorPool     = vEngine.descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &vk_vectorEngineDescriptorSetLayout;

        vk::DescriptorSet descriptorSet = vk_device.allocateDescriptorSets(allocInfo).back();

        vEngine.descriptorSet = descriptorSet;

        Logger::info("Created Vector Engine Descriptor Set");
    }

    inline void VulkanInterface::updateVectorEngineEdges(VectorEngineObject& vEngine) {
        vk::DescriptorBufferInfo edgesInBufferInfo;
        edgesInBufferInfo.buffer = vEngine.edgeBuffer;
        edgesInBufferInfo.offset = 0;
        edgesInBufferInfo.range  = vEngine.edges.size() * sizeof(VectorEdgeBufferObject);

        vk::DescriptorBufferInfo edgesOutBufferInfo;
        edgesOutBufferInfo.buffer = vEngine.transformedEdgeBuffer;
        edgesOutBufferInfo.offset = 0;
        edgesOutBufferInfo.range  = vEngine.edges.size() * sizeof(VectorEdgeBufferObject);

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites;
        
        descriptorWrites[0].dstSet          = vEngine.descriptorSet;
        descriptorWrites[0].dstBinding      = VectorEngineDescriporBindings::eEdgesIn;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eStorageBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &edgesInBufferInfo;

        descriptorWrites[1].dstSet          = vEngine.descriptorSet;
        descriptorWrites[1].dstBinding      = VectorEngineDescriporBindings::eEdgesOut;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType  = vk::DescriptorType::eStorageBuffer;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo     = &edgesOutBufferInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        Logger::info("Updated Vector Engine Edges");
    }

    inline void VulkanInterface::updateVectorEngineDescriptorSet(VectorEngineObject& vEngine) {
        SPLITGUI_PROFILE;

        vk::DescriptorBufferInfo sceneDataBufferInfo;
        sceneDataBufferInfo.buffer = scenes[vEngine.scene.sceneNumber].dataUniformBuffer;
        sceneDataBufferInfo.offset = 0;
        sceneDataBufferInfo.range  = sizeof(SceneObj);

        vk::DescriptorImageInfo outputImageInfo;
        outputImageInfo.sampler     = vEngine.outputImageSampler;
        outputImageInfo.imageView   = vEngine.outputImageView;
        outputImageInfo.imageLayout = vk::ImageLayout::eGeneral;

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites;

        descriptorWrites[0].dstSet          = vEngine.descriptorSet;
        descriptorWrites[0].dstBinding      = VectorEngineDescriporBindings::eSceneData;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &sceneDataBufferInfo;

        descriptorWrites[1].dstSet          = vEngine.descriptorSet;
        descriptorWrites[1].dstBinding      = VectorEngineDescriporBindings::eOutputImage;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType  = vk::DescriptorType::eStorageImage;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo      = &outputImageInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        Logger::info("Updated Vector Engine Descriptor Sets");
    }
}