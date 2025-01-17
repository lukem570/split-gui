#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::createRenderpass() {
        vk::AttachmentDescription colorAttachment;
        colorAttachment.format              = vk_surfaceFormat.format;
        colorAttachment.samples             = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp              = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp             = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout       = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout         = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference colorAttachmentReference;
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint           = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount        = 1;
        subpass.pColorAttachments           = &colorAttachmentReference;

        vk::SubpassDependency dependency;
        dependency.srcSubpass               = vk::SubpassExternal;
        dependency.dstSubpass               = 0;
        dependency.srcStageMask             = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask            = (vk::AccessFlags) 0;
        dependency.dstStageMask             = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstAccessMask           |= vk::AccessFlagBits::eColorAttachmentRead;
        dependency.dstAccessMask           |= vk::AccessFlagBits::eColorAttachmentWrite;

        vk::RenderPassCreateInfo createInfo;
        createInfo.attachmentCount          = 1;
        createInfo.subpassCount             = 1;
        createInfo.dependencyCount          = 1;
        createInfo.pAttachments             = &colorAttachment;
        createInfo.pSubpasses               = &subpass;
        createInfo.pDependencies            = &dependency;

        vk_renderpass = vk_device.createRenderPass(createInfo);
    }
}