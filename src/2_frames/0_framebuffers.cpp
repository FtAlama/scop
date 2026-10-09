#include "frames/0_framebuffers.hpp"
#include "vk_context.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

Framebuffers::Framebuffers(vk_context &ctx,
                           const std::vector<VkImageView> &swapChainImageViews,
                           const VkRenderPass renderPass,
                           const VkExtent2D swapChainExtent)
    : ctx(ctx) {
  swapchainFramebuffers.resize(swapChainImageViews.size());
  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {swapChainImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(ctx.device, &framebufferInfo, nullptr,
                            &swapchainFramebuffers[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create framebuffers!");
  }
}

Framebuffers::~Framebuffers() {
  for (auto framebuffer : swapchainFramebuffers)
    vkDestroyFramebuffer(ctx.device, framebuffer, nullptr);
}
