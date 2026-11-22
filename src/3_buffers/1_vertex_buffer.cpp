#include "buffers/0_creation_buffers.hpp"
#include "vertex.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

VertexBuffer::VertexBuffer(vk_context &ctx) : ctx(ctx) {
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
  create_buffer(ctx, bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                vertexBuffer, vertexBufferMemory);

  void *data;
  vkMapMemory(ctx.device, vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(ctx.device, vertexBufferMemory);
}

VertexBuffer::~VertexBuffer() {
  vkDestroyBuffer(ctx.device, vertexBuffer, nullptr);
  vkFreeMemory(ctx.device, vertexBufferMemory, nullptr);
}
