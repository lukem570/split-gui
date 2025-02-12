#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createGraphicsPipelineLayout() {
        vk::PipelineLayoutCreateInfo createInfo;
        createInfo.setLayoutCount         = 1;
        createInfo.pSetLayouts            = &vk_descriptorSetLayout;
        createInfo.pushConstantRangeCount = 0;

        vk_graphicsPipelineLayout = vk_device.createPipelineLayout(createInfo);

        SPLITGUI_LOG("Created Pipeline Layout");
    }

    inline Result VulkanInterface::createGraphicsPipeline() {

        ResultValue<std::vector<char>> vertexShaderFile   = readFile(VERTEX_SHADER_PATH);
        ResultValue<std::vector<char>> fragmentShaderFile = readFile(FRAGMENT_SHADER_PATH);

        TRYD(vertexShaderFile);
        TRYD(fragmentShaderFile);

        vk::ShaderModule vertexShader   = createShaderModule(vertexShaderFile.value);
        vk::ShaderModule fragmentShader = createShaderModule(fragmentShaderFile.value);

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
        
        std::array<vk::VertexInputAttributeDescription, 7> attributeDescriptions;
        attributeDescriptions[VertexLayout::eVertexPos].binding          = 0;
        attributeDescriptions[VertexLayout::eVertexPos].location         = VertexLayout::eVertexPos;
        attributeDescriptions[VertexLayout::eVertexPos].format           = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[VertexLayout::eVertexPos].offset           = offsetof(VertexBufferObject, VertexBufferObject::vertex.pos);

        attributeDescriptions[VertexLayout::eVertexColor].binding        = 0;
        attributeDescriptions[VertexLayout::eVertexColor].location       = VertexLayout::eVertexColor;
        attributeDescriptions[VertexLayout::eVertexColor].format         = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[VertexLayout::eVertexColor].offset         = offsetof(VertexBufferObject, VertexBufferObject::vertex.color);

        attributeDescriptions[VertexLayout::eVertexTextureCord].binding  = 0;
        attributeDescriptions[VertexLayout::eVertexTextureCord].location = VertexLayout::eVertexTextureCord;
        attributeDescriptions[VertexLayout::eVertexTextureCord].format   = vk::Format::eR32G32Sfloat;
        attributeDescriptions[VertexLayout::eVertexTextureCord].offset   = offsetof(VertexBufferObject, VertexBufferObject::vertex.textureCord);

        attributeDescriptions[VertexLayout::eFlags].binding              = 0;
        attributeDescriptions[VertexLayout::eFlags].location             = VertexLayout::eFlags;
        attributeDescriptions[VertexLayout::eFlags].format               = vk::Format::eR16Uint;
        attributeDescriptions[VertexLayout::eFlags].offset               = offsetof(VertexBufferObject, VertexBufferObject::flags);

        attributeDescriptions[VertexLayout::eSceneNumber].binding        = 0;
        attributeDescriptions[VertexLayout::eSceneNumber].location       = VertexLayout::eSceneNumber;
        attributeDescriptions[VertexLayout::eSceneNumber].format         = vk::Format::eR16Uint;
        attributeDescriptions[VertexLayout::eSceneNumber].offset         = offsetof(VertexBufferObject, VertexBufferObject::sceneNumber);

        attributeDescriptions[VertexLayout::eTextureNumber].binding      = 0;
        attributeDescriptions[VertexLayout::eTextureNumber].location     = VertexLayout::eTextureNumber;
        attributeDescriptions[VertexLayout::eTextureNumber].format       = vk::Format::eR16Uint;
        attributeDescriptions[VertexLayout::eTextureNumber].offset       = offsetof(VertexBufferObject, VertexBufferObject::textureNumber);

        attributeDescriptions[VertexLayout::eNormal].binding             = 0;
        attributeDescriptions[VertexLayout::eNormal].location            = VertexLayout::eNormal;
        attributeDescriptions[VertexLayout::eNormal].format              = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[VertexLayout::eNormal].offset              = offsetof(VertexBufferObject, VertexBufferObject::normal);

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
        colorBlendAttachment.blendEnable         = vk::True;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.colorBlendOp        = vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.alphaBlendOp        = vk::BlendOp::eAdd;

        vk::PipelineColorBlendStateCreateInfo colorBlending;
        colorBlending.logicOpEnable   = vk::False;
        colorBlending.logicOp         = vk::LogicOp::eCopy;
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
            return Result::eFailedToCreateGraphicsPipeline;
        } 

        vk_device.destroyShaderModule(fragmentShader);
        vk_device.destroyShaderModule(vertexShader);

        vk_graphicsPipeline = result.value;

        SPLITGUI_LOG("Created Graphics Pipeline");

        return Result::eSuccess;
    }
}