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
}