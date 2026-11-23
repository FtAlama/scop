#include "buffers/0_creation_buffers.hpp"
#include "vertex.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

VertexBuffer::VertexBuffer(vk_context &ctx) : ctx(ctx) {

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	create_buffer(ctx, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(ctx.device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(ctx.device, stagingBufferMemory);

  create_buffer(
      ctx, bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
	copyBuffer(stagingBuffer, vertexBuffer, bufferSize, ctx);
	vkDestroyBuffer(ctx.device, stagingBuffer, nullptr);
	vkFreeMemory(ctx.device, stagingBufferMemory, nullptr);
}

VertexBuffer::~VertexBuffer() {
  vkDestroyBuffer(ctx.device, vertexBuffer, nullptr);
  vkFreeMemory(ctx.device, vertexBufferMemory, nullptr);
}
