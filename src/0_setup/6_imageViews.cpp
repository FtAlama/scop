#include "setup/6_imageViews.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

ImageViews::ImageViews(vk_context &ctx,
                       std::vector<VkImage> const &swapChainImages,
                       VkFormat const swapChainImageFormat)
    : ctx(ctx) {
  swapChainImageViews.resize(swapChainImages.size());

  for (uint32_t i = 0; i < swapChainImages.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(ctx.device, &createInfo, nullptr,
                          &swapChainImageViews[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create image views!");
  }
}

ImageViews::~ImageViews() {
  for (auto imageView : swapChainImageViews) {
    vkDestroyImageView(ctx.device, imageView, nullptr);
  }
}
