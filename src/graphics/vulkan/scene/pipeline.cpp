#include "../vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createScenePipelineLayout() {
        vk::PipelineLayoutCreateInfo createInfo;
        createInfo.setLayoutCount         = 1;
        createInfo.pSetLayouts            = &vk_sceneDescriptorSetLayout;
        createInfo.pushConstantRangeCount = 0;

        vk::PipelineLayout pipelineLayout = vk_device.createPipelineLayout(createInfo);

        vk_scenePipelineLayout = pipelineLayout;

        SPLITGUI_LOG("Created Scene Pipeline Layout");
    }

    inline Result VulkanInterface::createScenePipelineModules() {

        ResultValue<std::vector<char>> vertexShaderFile   = readFile(SCENE_VERTEX_SHADER_PATH);
        ResultValue<std::vector<char>> fragmentShaderFile = readFile(SCENE_FRAGMENT_SHADER_PATH);

        TRYD(vertexShaderFile);
        TRYD(fragmentShaderFile);

        vk_sceneVertexModule   = createShaderModule(vertexShaderFile.value);
        vk_sceneFragmentModule = createShaderModule(fragmentShaderFile.value);

        return Result::eSuccess;
    }

    inline Result VulkanInterface::createScenePipeline(SceneObject& scene) {

        vk::PipelineShaderStageCreateInfo vertexCreateInfo;
        vertexCreateInfo.stage  = vk::ShaderStageFlagBits::eVertex;
        vertexCreateInfo.module = vk_sceneVertexModule;
        vertexCreateInfo.pName  = "main";

        vk::PipelineShaderStageCreateInfo fragmentCreateInfo;
        fragmentCreateInfo.stage  = vk::ShaderStageFlagBits::eFragment;
        fragmentCreateInfo.module = vk_sceneFragmentModule;
        fragmentCreateInfo.pName  = "main";

        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertexCreateInfo, fragmentCreateInfo};

        std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicState;
        dynamicState.dynamicStateCount = dynamicStates.size();
        dynamicState.pDynamicStates    = dynamicStates.data();

        vk::VertexInputBindingDescription bindingDescription;
        bindingDescription.binding   = 0;
        bindingDescription.stride    = sizeof(SceneVertexBufferObject);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        
        std::array<vk::VertexInputAttributeDescription, 7> attributeDescriptions;
        attributeDescriptions[SceneVertexLayout::eVertexPos].binding          = 0;
        attributeDescriptions[SceneVertexLayout::eVertexPos].location         = SceneVertexLayout::eVertexPos;
        attributeDescriptions[SceneVertexLayout::eVertexPos].format           = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[SceneVertexLayout::eVertexPos].offset           = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::vertex.pos);

        attributeDescriptions[SceneVertexLayout::eVertexColor].binding        = 0;
        attributeDescriptions[SceneVertexLayout::eVertexColor].location       = SceneVertexLayout::eVertexColor;
        attributeDescriptions[SceneVertexLayout::eVertexColor].format         = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[SceneVertexLayout::eVertexColor].offset         = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::vertex.color);

        attributeDescriptions[SceneVertexLayout::eVertexTextureCord].binding  = 0;
        attributeDescriptions[SceneVertexLayout::eVertexTextureCord].location = SceneVertexLayout::eVertexTextureCord;
        attributeDescriptions[SceneVertexLayout::eVertexTextureCord].format   = vk::Format::eR32G32Sfloat;
        attributeDescriptions[SceneVertexLayout::eVertexTextureCord].offset   = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::vertex.textureCord);

        attributeDescriptions[SceneVertexLayout::eFlags].binding              = 0;
        attributeDescriptions[SceneVertexLayout::eFlags].location             = SceneVertexLayout::eFlags;
        attributeDescriptions[SceneVertexLayout::eFlags].format               = vk::Format::eR16Uint;
        attributeDescriptions[SceneVertexLayout::eFlags].offset               = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::flags);

        attributeDescriptions[SceneVertexLayout::eTextureNumber].binding      = 0;
        attributeDescriptions[SceneVertexLayout::eTextureNumber].location     = SceneVertexLayout::eTextureNumber;
        attributeDescriptions[SceneVertexLayout::eTextureNumber].format       = vk::Format::eR16Uint;
        attributeDescriptions[SceneVertexLayout::eTextureNumber].offset       = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::textureNumber);

        attributeDescriptions[SceneVertexLayout::eModelNumber].binding        = 0;
        attributeDescriptions[SceneVertexLayout::eModelNumber].location       = SceneVertexLayout::eModelNumber;
        attributeDescriptions[SceneVertexLayout::eModelNumber].format         = vk::Format::eR16Uint;
        attributeDescriptions[SceneVertexLayout::eModelNumber].offset         = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::modelNumber);

        attributeDescriptions[SceneVertexLayout::eNormal].binding             = 0;
        attributeDescriptions[SceneVertexLayout::eNormal].location            = SceneVertexLayout::eNormal;
        attributeDescriptions[SceneVertexLayout::eNormal].format              = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[SceneVertexLayout::eNormal].offset              = offsetof(SceneVertexBufferObject, SceneVertexBufferObject::normal);

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
        rasterizer.depthBiasEnable         = vk::True;

        vk::PipelineMultisampleStateCreateInfo multisampling;
        multisampling.sampleShadingEnable  = vk::False;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.blendEnable         = vk::True;
        colorBlendAttachment.colorWriteMask      = vk::ColorComponentFlagBits::eR;
        colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eG;
        colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eB;
        colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eA;
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

        vk::PipelineDepthStencilStateCreateInfo depthStencil;
        depthStencil.depthTestEnable       = vk::True;
        depthStencil.depthWriteEnable      = vk::True;
        depthStencil.depthCompareOp        = vk::CompareOp::eLess;
        depthStencil.depthBoundsTestEnable = vk::False;
        depthStencil.stencilTestEnable     = vk::False;
        depthStencil.minDepthBounds        = 0.0f;
        depthStencil.maxDepthBounds        = 1.0f;

        vk::GraphicsPipelineCreateInfo pipelineInfo;
        pipelineInfo.stageCount          = shaderStages.size();
        pipelineInfo.pStages             = shaderStages.data();
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDepthStencilState  = &depthStencil;
        pipelineInfo.pDynamicState       = &dynamicState;
        pipelineInfo.layout              = vk_scenePipelineLayout;
        pipelineInfo.renderPass          = vk_renderpass;
        pipelineInfo.subpass             = 0;
        pipelineInfo.basePipelineHandle  = nullptr;
        pipelineInfo.basePipelineIndex   = -1;

        vk::ResultValue<vk::Pipeline> result = vk_device.createGraphicsPipeline(nullptr, pipelineInfo);

        if (result.result != vk::Result::eSuccess) {
            return Result::eFailedToCreateGraphicsPipeline;
        } 

        scene.pipeline = result.value;

        SPLITGUI_LOG("Created Scene Graphics Pipeline");

        return Result::eSuccess;
    }
}