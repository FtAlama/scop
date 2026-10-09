#include "frames/2_syncObjects.hpp"
#include "vk_context.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

SyncObjects::SyncObjects(vk_context &ctx,
                         const std::vector<VkImageView> &swapChainImageViews)
    : ctx(ctx) {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  renderFinishedSemaphores.resize(swapChainImageViews.size());
  for (uint32_t i = 0; i < swapChainImageViews.size(); ++i)
    if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create semaphores!");
  if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                        &imageAvailableSemaphore) != VK_SUCCESS ||
      vkCreateFence(ctx.device, &fenceInfo, nullptr, &inFlightFence) !=
          VK_SUCCESS)
    throw std::runtime_error("failed to create semaphores!");
}

SyncObjects::~SyncObjects() {
  for (uint32_t i = 0; i < renderFinishedSemaphores.size(); ++i)
    vkDestroySemaphore(ctx.device, renderFinishedSemaphores[i], nullptr);
  vkDestroySemaphore(ctx.device, imageAvailableSemaphore, nullptr);
  vkDestroyFence(ctx.device, inFlightFence, nullptr);
}
