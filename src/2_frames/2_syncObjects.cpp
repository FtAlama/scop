#include "frames/2_syncObjects.hpp"
#include "vk_context.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

SyncObjects::SyncObjects(vk_context &ctx) : ctx(ctx) {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  ctx.renderFinishedSemaphores.resize(ctx.swapChainImageViews.size());
  for (uint32_t i = 0; i < ctx.swapChainImageViews.size(); ++i)
    if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                          &ctx.renderFinishedSemaphores[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create semaphores!");
  if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                        &ctx.imageAvailableSemaphore) != VK_SUCCESS ||
      vkCreateFence(ctx.device, &fenceInfo, nullptr, &ctx.inFlightFence) !=
          VK_SUCCESS)
    throw std::runtime_error("failed to create semaphores!");
}

SyncObjects::~SyncObjects() {
  for (uint32_t i = 0; i < ctx.renderFinishedSemaphores.size(); ++i)
    vkDestroySemaphore(ctx.device, ctx.renderFinishedSemaphores[i], nullptr);
  vkDestroySemaphore(ctx.device, ctx.imageAvailableSemaphore, nullptr);
  vkDestroyFence(ctx.device, ctx.inFlightFence, nullptr);
}
