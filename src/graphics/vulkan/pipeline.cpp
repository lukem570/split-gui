#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createGraphicsPipelineLayout() {
        vk::PipelineLayoutCreateInfo createInfo;
        createInfo.setLayoutCount         = 1;
        createInfo.pSetLayouts            = &vk_descriptorSetLayout;
        //createInfo.setLayoutCount         = 0;
        //createInfo.pSetLayouts            = nullptr;
        createInfo.pushConstantRangeCount = 0;

        vk_graphicsPipelineLayout = vk_device.createPipelineLayout(createInfo);
    }

    inline void VulkanInterface::createGraphicsPipeline() {

        const std::vector<char> vertexShaderFile   = readFile(VERTEX_SHADER_PATH);
        const std::vector<char> fragmentShaderFile = readFile(FRAGMENT_SHADER_PATH);

        vk::ShaderModule vertexShader   = createShaderModule(vertexShaderFile);
        vk::ShaderModule fragmentShader = createShaderModule(fragmentShaderFile);

        vk::PipelineShaderStageCreateInfo vertexCreateInfo;
        vertexCreateInfo.stage  = vk::ShaderStageFlagBits::eVertex;
        vertexCreateInfo.module = vertexShader;
        vertexCreateInfo.pName  = "main";

        vk::PipelineShaderStageCreateInfo fragmentCreateInfo;
        fragmentCreateInfo.stage  = vk::ShaderStageFlagBits::eFragment;
        fragmentCreateInfo.module = fragmentShader;
        fragmentCreateInfo.pName  = "main";

        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertexCreateInfo, fragmentCreateInfo};

        std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicState;
        dynamicState.dynamicStateCount = dynamicStates.size();
        dynamicState.pDynamicStates    = dynamicStates.data();

        vk::VertexInputBindingDescription bindingDescription;
        bindingDescription.binding   = 0;
        bindingDescription.stride    = sizeof(VertexBufferObject);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        
        std::array<vk::VertexInputAttributeDescription, 5> attributeDescriptions;
        attributeDescriptions[0].binding  = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format   = vk::Format::eR32G32Sfloat;
        attributeDescriptions[0].offset   = offsetof(VertexBufferObject, VertexBufferObject::vertex.pos);

        attributeDescriptions[1].binding  = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format   = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset   = offsetof(VertexBufferObject, VertexBufferObject::vertex.color);

        attributeDescriptions[2].binding  = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format   = vk::Format::eR16Uint;
        attributeDescriptions[2].offset   = offsetof(VertexBufferObject, VertexBufferObject::flags);

        attributeDescriptions[3].binding  = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format   = vk::Format::eR16Uint;
        attributeDescriptions[3].offset   = offsetof(VertexBufferObject, VertexBufferObject::sceneNumber);

        attributeDescriptions[4].binding  = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format   = vk::Format::eR16Uint;
        attributeDescriptions[4].offset   = offsetof(VertexBufferObject, VertexBufferObject::textureNumber);

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
        vertexInputInfo.vertexBindingDescriptionCount   = 1;
        vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
        
        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
        inputAssembly.topology               = vk::PrimitiveTopology::eTriangleList;
        inputAssembly.primitiveRestartEnable = vk::False;

        vk::PipelineViewportStateCreateInfo viewportState;
        viewportState.viewportCount = 1;
        viewportState.scissorCount  = 1;

        vk::PipelineRasterizationStateCreateInfo rasterizer;
        rasterizer.depthClampEnable        = vk::False;
        rasterizer.rasterizerDiscardEnable = vk::False;
        rasterizer.polygonMode             = vk::PolygonMode::eFill;
        rasterizer.lineWidth               = 1.0f;
        rasterizer.cullMode                = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace               = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable         = vk::False;

        vk::PipelineMultisampleStateCreateInfo multisampling;
        multisampling.sampleShadingEnable  = vk::False;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.colorWriteMask      = vk::ColorComponentFlagBits::eR;
        colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eG;
        colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eB;
        colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable         = vk::False;

        vk::PipelineColorBlendStateCreateInfo colorBlending;
        colorBlending.logicOpEnable   = vk::False;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments    = &colorBlendAttachment;

        vk::GraphicsPipelineCreateInfo pipelineInfo;
        pipelineInfo.stageCount          = shaderStages.size();
        pipelineInfo.pStages             = shaderStages.data();
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDynamicState       = &dynamicState;
        pipelineInfo.layout              = vk_graphicsPipelineLayout;
        pipelineInfo.renderPass          = vk_renderpass;
        pipelineInfo.subpass             = 0;
        pipelineInfo.basePipelineHandle  = nullptr;
        pipelineInfo.basePipelineIndex   = -1;

        vk::ResultValue<vk::Pipeline> result = vk_device.createGraphicsPipeline(nullptr, pipelineInfo);

        if (result.result != vk::Result::eSuccess) {
            printf("Error: error creating graphics pipeline\n");
            throw;
        } 

        vk_device.destroyShaderModule(fragmentShader);
        vk_device.destroyShaderModule(vertexShader);

        vk_graphicsPipeline = result.value;
    }
}