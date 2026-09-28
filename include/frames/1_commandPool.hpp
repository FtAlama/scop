#ifndef COMMANDPOOL_HPP

#define COMMANDPOOL_HPP

#include "vk_context.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

class CommandPool {
public:
  CommandPool(vk_context &);
  ~CommandPool();

private:
  vk_context &ctx;
};

class CommandBuffer {
public:
  CommandBuffer(vk_context &);
  ~CommandBuffer();

private:
  vk_context &ctx;
  void recordCommandBuffer(uint32_t);
};

#endif
