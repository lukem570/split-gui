#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::createRenderpass() {
        SPLITGUI_PROFILE;

        ResultValue<vk::Format> depthFormat = findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );

        TRYD(depthFormat);

        vk::AttachmentDescription colorAttachment;
        colorAttachment.format              = vk_surfaceFormat.format;
        colorAttachment.samples             = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp              = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp             = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout       = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout         = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentDescription depthAttachment;
        depthAttachment.format              = depthFormat.value;
        depthAttachment.samples             = vk::SampleCountFlagBits::e1;
        depthAttachment.loadOp              = vk::AttachmentLoadOp::eClear;
        depthAttachment.storeOp             = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
        depthAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.initialLayout       = vk::ImageLayout::eUndefined;
        depthAttachment.finalLayout         = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

        vk::AttachmentReference colorAttachmentReference;
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference depthAttachmentReference;
        depthAttachmentReference.attachment = 1;
        depthAttachmentReference.layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint           = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount        = 1;
        subpass.pColorAttachments           = &colorAttachmentReference;
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

        Logger::info("Created Renderpass");

        return Result::eSuccess;
    }
}