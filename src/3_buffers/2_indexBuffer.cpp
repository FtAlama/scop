#include "buffers/0_creationBuffers.hpp"
#include "vertex.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

IndexBuffer::IndexBuffer(vk_context &ctx) : ctx(ctx) {
  VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  create_buffer(ctx, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);
  void *data;
  vkMapMemory(ctx.device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(ctx.device, stagingBufferMemory);

  create_buffer(
      ctx, bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
  copyBuffer(stagingBuffer, indexBuffer, bufferSize, ctx);
  vkDestroyBuffer(ctx.device, stagingBuffer, nullptr);
  vkFreeMemory(ctx.device, stagingBufferMemory, nullptr);
}

IndexBuffer::~IndexBuffer() {
	vkDestroyBuffer(ctx.device, indexBuffer, nullptr);
	vkFreeMemory(ctx.device, indexBufferMemory, nullptr);
}
