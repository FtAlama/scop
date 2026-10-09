#ifndef COMMANDPOOL_HPP

#define COMMANDPOOL_HPP

#include "vk_context.hpp"
#include <cstdint>
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
  VkCommandBuffer get() const { return commandBuffer; }
  ~CommandBuffer();
  void recordCommandBuffer(const uint32_t, const VkRenderPass,
                           const std::vector<VkFramebuffer> &, const VkExtent2D,
                           const VkPipeline);

private:
  vk_context &ctx;
  VkCommandBuffer commandBuffer;
};

#endif
