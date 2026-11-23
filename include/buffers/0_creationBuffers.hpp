#ifndef CREATION_BUFFERS_HPP

#define CREATION_BUFFERS_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan_core.h>

class MeshBuffer {
public:
  MeshBuffer(vk_context &);
  ~MeshBuffer();
  const VkBuffer &get() const { return meshBuffer; }

private:
  vk_context &ctx;
  VkBuffer meshBuffer;
  VkDeviceMemory meshBufferMemory;
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
