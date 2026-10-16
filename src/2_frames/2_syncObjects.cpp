#include "frames/2_syncObjects.hpp"
#include "vk_context.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

SyncObjects::SyncObjects(vk_context &ctx, const uint32_t image_nb) : ctx(ctx) {
  frames.resize(MAX_FRAMES_IN_FLIGHT);
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                          &frames[i].imageAvailableSemaphore) != VK_SUCCESS) {
      for (uint32_t j = 0; j < i; ++j)
        vkDestroySemaphore(ctx.device, frames[j].imageAvailableSemaphore,
                           nullptr);
      throw std::runtime_error("failed to create semaphores!");
    }
  }
  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (vkCreateFence(ctx.device, &fenceInfo, nullptr,
                      &frames[i].inFlightFence) != VK_SUCCESS) {
      for (uint32_t j = 0; j < MAX_FRAMES_IN_FLIGHT; ++j)
        vkDestroySemaphore(ctx.device, frames[j].imageAvailableSemaphore,
                           nullptr);
      for (uint32_t j = 0; j < i; ++j)
        vkDestroyFence(ctx.device, frames[j].inFlightFence, nullptr);

      throw std::runtime_error("failed to create semaphores!");
    }
  }
  for (uint32_t i = 0; i < image_nb; ++i)
    if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                          &frames[i].renderFinishedSemaphores) != VK_SUCCESS) {

      for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(ctx.device, frames[i].imageAvailableSemaphore,
                           nullptr);
        vkDestroyFence(ctx.device, frames[i].inFlightFence, nullptr);
      }
      for (uint32_t j = 0; j < i; ++j)
        vkDestroySemaphore(ctx.device, frames[j].renderFinishedSemaphores, nullptr);
      throw std::runtime_error("failed to create semaphores!");
    }
}

void SyncObjects::recreateRenderFinished() {
  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    vkDestroySemaphore(ctx.device, frames[i].renderFinishedSemaphores, nullptr);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    if (vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr,
                          &frames[i].renderFinishedSemaphores) != VK_SUCCESS)
      throw std::runtime_error("failed to create semaphores!");
}

SyncObjects::~SyncObjects() {
  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    vkDestroySemaphore(ctx.device, frames[i].imageAvailableSemaphore, nullptr);
    vkDestroyFence(ctx.device, frames[i].inFlightFence, nullptr);
    vkDestroySemaphore(ctx.device, frames[i].renderFinishedSemaphores, nullptr);
  }
}
