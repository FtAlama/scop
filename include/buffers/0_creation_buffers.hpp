#ifndef CREATION_BUFFERS_HPP

#define CREATION_BUFFERS_HPP

#include "vk_context.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>
class VertexBuffer {
public:
  VertexBuffer(vk_context &);
  ~VertexBuffer();
	const VkBuffer& get() const { return vertexBuffer; }

private:
  vk_context &ctx;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkMemoryRequirements memRequirements;
  uint32_t findMemoryType(uint32_t, const VkMemoryPropertyFlags);
};

#endif
