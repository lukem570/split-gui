#include "../vulkan.hpp"

namespace SplitGui {

    inline void VulkanInterface::createVectorEnginePipelineLayout() {
        SPLITGUI_PROFILE;
        
        vk::PipelineLayoutCreateInfo createInfo;
        createInfo.setLayoutCount         = 1;
        createInfo.pSetLayouts            = &vk_vectorEngineDescriptorSetLayout;
        createInfo.pushConstantRangeCount = 0;

        vk::PipelineLayout pipelineLayout = vk_device.createPipelineLayout(createInfo);

        vk_vectorEnginePipelineLayout = pipelineLayout;

        Logger::info("Created Vector Engine Pipeline Layout");
    }

    inline Result VulkanInterface::createVectorEnginePipelineModules() {
        SPLITGUI_PROFILE;

        ResultValue<std::vector<char>> transformShaderFile = readFile(VECTOR_ENGINE_TRANSFORM_SHADER_PATH);
        ResultValue<std::vector<char>> renderShaderFile    = readFile(VECTOR_ENGINE_RENDER_SHADER_PATH);

        TRYD(transformShaderFile);
        TRYD(renderShaderFile);

        vk_vectorEngineTransformModule = createShaderModule(transformShaderFile.value);
        vk_vectorEngineRenderModule    = createShaderModule(renderShaderFile.value);

        return Result::eSuccess;
    }

    inline Result VulkanInterface::createVectorEngineTransformPipeline(VectorEngineObject& vEngine) {
        SPLITGUI_PROFILE;

        vk::PhysicalDeviceProperties properties = vk_physicalDevice.getProperties();

        vectorTransformPassSize = properties.limits.maxComputeWorkGroupInvocations;

        Logger::info("Vector Transform Work Group Size " + std::to_string(vectorTransformPassSize));

        vk::SpecializationMapEntry xSizeEntry;
        xSizeEntry.constantID = 0;
        xSizeEntry.size       = sizeof(vectorTransformPassSize);
        xSizeEntry.offset     = 0;

        vk::SpecializationInfo specInfo;
        specInfo.mapEntryCount = 1;
        specInfo.pMapEntries   = &xSizeEntry;
        specInfo.dataSize      = sizeof(vectorTransformPassSize);
        specInfo.pData         = &vectorTransformPassSize;

        vk::PipelineShaderStageCreateInfo shaderStage;
        shaderStage.stage               = vk::ShaderStageFlagBits::eCompute;
        shaderStage.module              = vk_vectorEngineTransformModule;
        shaderStage.pSpecializationInfo = &specInfo;
        shaderStage.pName               = "main";

        vk::ComputePipelineCreateInfo pipelineInfo;
        pipelineInfo.stage  = shaderStage;
        pipelineInfo.layout = vk_vectorEnginePipelineLayout;

        vk::ResultValue<vk::Pipeline> result = vk_device.createComputePipeline(nullptr, pipelineInfo);

        if (result.result != vk::Result::eSuccess) {
            return Result::eFailedToCreateComputePipeline;
        } 

        vEngine.transformPipeline = result.value;

        Logger::info("Created Vector Engine Transform Pipeline");

        return Result::eSuccess;
    }

    inline Result VulkanInterface::createVectorEngineRenderPipeline(VectorEngineObject& vEngine) {
        SPLITGUI_PROFILE;

        vk::PhysicalDeviceProperties properties = vk_physicalDevice.getProperties();

        uint32_t maxInvocations = properties.limits.maxComputeWorkGroupInvocations;

        vectorRenderPassSize.x = std::ceil(std::sqrt((float) maxInvocations));
        vectorRenderPassSize.y = std::floor(std::sqrt((float) maxInvocations));

        vk::SpecializationMapEntry xSizeEntry;
        xSizeEntry.constantID = 0;
        xSizeEntry.size       = sizeof(IVec2::x);
        xSizeEntry.offset     = offsetof(IVec2, IVec2::x);

        vk::SpecializationMapEntry ySizeEntry;
        ySizeEntry.constantID = 1;
        ySizeEntry.size       = sizeof(IVec2::y);
        ySizeEntry.offset     = offsetof(IVec2, IVec2::y);

        std::array<vk::SpecializationMapEntry, 2> entries = {xSizeEntry, ySizeEntry};

        vk::SpecializationInfo specInfo;
        specInfo.mapEntryCount = entries.size();
        specInfo.pMapEntries   = entries.data();
        specInfo.dataSize      = sizeof(vectorRenderPassSize);
        specInfo.pData         = &vectorRenderPassSize;

        vk::PipelineShaderStageCreateInfo shaderStage;
        shaderStage.stage               = vk::ShaderStageFlagBits::eCompute;
        shaderStage.module              = vk_vectorEngineRenderModule;
        shaderStage.pSpecializationInfo = &specInfo;
        shaderStage.pName               = "main";

        vk::ComputePipelineCreateInfo pipelineInfo;
        pipelineInfo.stage  = shaderStage;
        pipelineInfo.layout = vk_vectorEnginePipelineLayout;

        vk::ResultValue<vk::Pipeline> result = vk_device.createComputePipeline(nullptr, pipelineInfo);

        if (result.result != vk::Result::eSuccess) {
            return Result::eFailedToCreateComputePipeline;
        } 

        vEngine.renderPipeline = result.value;

        Logger::info("Created Vector Engine Render Pipeline");

        return Result::eSuccess;
    }

}