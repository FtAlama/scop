#ifndef COMMANDPOOL_HPP

#define COMMANDPOOL_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

class CommandPool {
public:
  CommandPool(vk_context &);
  VkCommandPool get() const { return commandPool; }
  ~CommandPool();

private:
  vk_context &ctx;
  VkCommandPool commandPool = VK_NULL_HANDLE;
};

class CommandBuffer {
public:
  CommandBuffer(vk_context &, const VkCommandPool);
  const std::vector<VkCommandBuffer> &get() const { return commandBuffers; }
  ~CommandBuffer();
  void recordCommandBuffer(const VkCommandBuffer, const VkFramebuffer,
                           const VkRenderPass, const VkExtent2D,
                           const VkPipeline, const VkBuffer &);

private:
  vk_context &ctx;
  std::vector<VkCommandBuffer> commandBuffers;
};

#endif
