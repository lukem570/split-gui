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
}