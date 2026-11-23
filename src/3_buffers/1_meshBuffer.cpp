#include "buffers/0_creationBuffers.hpp"
#include "vertex.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

MeshBuffer::MeshBuffer(vk_context &ctx) : ctx(ctx) {
  VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();
  VkDeviceSize indexSize = sizeof(indices[0]) * indices.size();
  VkDeviceSize meshSize = vertexSize + indexSize;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  create_buffer(ctx, meshSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(ctx.device, stagingBufferMemory, 0, meshSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)vertexSize);
  memcpy(static_cast<char *>(data) + vertexSize, indices.data(),
         (size_t)indexSize);
  vkUnmapMemory(ctx.device, stagingBufferMemory);

  create_buffer(
      ctx, meshSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, meshBuffer, meshBufferMemory);
  copyBuffer(stagingBuffer, meshBuffer, meshSize, ctx);
  vkDestroyBuffer(ctx.device, stagingBuffer, nullptr);
  vkFreeMemory(ctx.device, stagingBufferMemory, nullptr);
}

MeshBuffer::~MeshBuffer() {
  vkDestroyBuffer(ctx.device, meshBuffer, nullptr);
  vkFreeMemory(ctx.device, meshBufferMemory, nullptr);
}
