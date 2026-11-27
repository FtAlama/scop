#include "buffers/0_creationBuffers.hpp"
#include "vertex.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

MeshBuffer::MeshBuffer(vk_context &ctx)
    : ctx(ctx), meshBufferMemory(ctx), meshBuffer(ctx) {
  VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();
  VkDeviceSize indexSize = sizeof(indices[0]) * indices.size();
  VkDeviceSize meshSize = vertexSize + indexSize;

  StagingBuffer stagingBuffers(ctx);

  stagingBuffers.create(meshSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  void *data;
  vkMapMemory(ctx.device, stagingBuffers.getMemory(), 0, meshSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)vertexSize);
  memcpy(static_cast<char *>(data) + vertexSize, indices.data(),
         (size_t)indexSize);
  vkUnmapMemory(ctx.device, stagingBuffers.getMemory());

  meshBuffer.create(meshSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                  VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
  meshBufferMemory.create(meshBuffer.get(),
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  copyBuffer(stagingBuffers.get(), meshBuffer.get(), meshSize, ctx);
}

MeshBuffer::~MeshBuffer() {}
