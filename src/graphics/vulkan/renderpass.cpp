#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::createRenderpass() {

        ResultValue<vk::Format> depthFormat = findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );

        TRYD(depthFormat);

        vk::AttachmentDescription colorAccumAttachment;
        colorAccumAttachment.format              = vk::Format::eR16G16B16A16Sfloat;
        colorAccumAttachment.samples             = vk::SampleCountFlagBits::e1;
        colorAccumAttachment.loadOp              = vk::AttachmentLoadOp::eLoad;
        colorAccumAttachment.storeOp             = vk::AttachmentStoreOp::eStore;
        colorAccumAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
        colorAccumAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
        colorAccumAttachment.initialLayout       = vk::ImageLayout::eColorAttachmentOptimal;
        colorAccumAttachment.finalLayout         = vk::ImageLayout::eShaderReadOnlyOptimal;

        vk::AttachmentDescription weightAccumAttachment;
        weightAccumAttachment.format              = vk::Format::eR16Sfloat;
        weightAccumAttachment.samples             = vk::SampleCountFlagBits::e1;
        weightAccumAttachment.loadOp              = vk::AttachmentLoadOp::eLoad;
        weightAccumAttachment.storeOp             = vk::AttachmentStoreOp::eStore;
        weightAccumAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
        weightAccumAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
        weightAccumAttachment.initialLayout       = vk::ImageLayout::eColorAttachmentOptimal;
        weightAccumAttachment.finalLayout         = vk::ImageLayout::eShaderReadOnlyOptimal;

        vk::AttachmentDescription depthAttachment;
        depthAttachment.format              = depthFormat.value;
        depthAttachment.samples             = vk::SampleCountFlagBits::e1;
        depthAttachment.loadOp              = vk::AttachmentLoadOp::eClear;
        depthAttachment.storeOp             = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
        depthAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.initialLayout       = vk::ImageLayout::eUndefined;
        depthAttachment.finalLayout         = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        std::array<vk::AttachmentDescription, 3> attachments = {colorAccumAttachment, weightAccumAttachment, depthAttachment};

        vk::AttachmentReference colorAccumAttachmentReference;
        colorAccumAttachmentReference.attachment = 0;
        colorAccumAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference weightAccumAttachmentReference;
        weightAccumAttachmentReference.attachment = 1;
        weightAccumAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference depthAttachmentReference;
        depthAttachmentReference.attachment = 2;
        depthAttachmentReference.layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        std::array<vk::AttachmentReference, 2> colorAttachments = { colorAccumAttachmentReference, weightAccumAttachmentReference };

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint           = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount        = colorAttachments.size();
        subpass.pColorAttachments           = colorAttachments.data();
        subpass.pDepthStencilAttachment     = &depthAttachmentReference;

        vk::SubpassDependency dependency;
        dependency.srcSubpass               = vk::SubpassExternal;
        dependency.dstSubpass               = 0;
        dependency.srcStageMask             = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcStageMask            |= vk::PipelineStageFlagBits::eEarlyFragmentTests;
        dependency.srcAccessMask            = (vk::AccessFlags) 0;
        dependency.dstStageMask             = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstStageMask            |= vk::PipelineStageFlagBits::eEarlyFragmentTests;
        dependency.dstAccessMask            = vk::AccessFlagBits::eDepthStencilAttachmentRead;
        dependency.dstAccessMask           |= vk::AccessFlagBits::eColorAttachmentWrite;

        vk::RenderPassCreateInfo createInfo;
        createInfo.subpassCount             = 1;
        createInfo.dependencyCount          = 1;
        createInfo.pSubpasses               = &subpass;
        createInfo.pDependencies            = &dependency;
        createInfo.attachmentCount          = attachments.size();
        createInfo.pAttachments             = attachments.data();

        vk_renderpass = vk_device.createRenderPass(createInfo);

        SPLITGUI_LOG("Created Renderpass");

        return Result::eSuccess;
    }
}