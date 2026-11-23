#ifndef CREATION_BUFFERS_HPP

#define CREATION_BUFFERS_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan_core.h>

class VertexBuffer {
public:
  VertexBuffer(vk_context &);
  ~VertexBuffer();
  const VkBuffer &get() const { return vertexBuffer; }

private:
  vk_context &ctx;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
};

void create_buffer(vk_context &, VkDeviceSize, VkBufferUsageFlags,
                   VkMemoryPropertyFlags, VkBuffer &, VkDeviceMemory &);

void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize, vk_context &);

class IndexBuffer {
public:
  IndexBuffer(vk_context &);
  ~IndexBuffer();
  const VkBuffer &get() const { return indexBuffer; }

private:
  vk_context &ctx;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
};

#endif
