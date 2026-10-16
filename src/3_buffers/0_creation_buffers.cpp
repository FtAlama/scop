#include "buffers/0_creation_buffers.hpp"
#include "vertex.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter,
                                      const VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(ctx.physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }
  vkDestroyBuffer(ctx.device, vertexBuffer, nullptr);
  throw std::runtime_error("failed to find suitable memory type!");
}

VertexBuffer::VertexBuffer(vk_context &ctx) : ctx(ctx) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(ctx.device, &bufferInfo, nullptr, &vertexBuffer) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create vertex buffer!");

  vkGetBufferMemoryRequirements(ctx.device, vertexBuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(
      memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (vkAllocateMemory(ctx.device, &allocInfo, nullptr, &vertexBufferMemory) !=
      VK_SUCCESS) {
    vkDestroyBuffer(ctx.device, vertexBuffer, nullptr);
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  }
  vkBindBufferMemory(ctx.device, vertexBuffer, vertexBufferMemory, 0);
	void *data;
	vkMapMemory(ctx.device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices.data(), (size_t) bufferInfo.size);
	vkUnmapMemory(ctx.device, vertexBufferMemory);

}

VertexBuffer::~VertexBuffer() {
  vkDestroyBuffer(ctx.device, vertexBuffer, nullptr);
  vkFreeMemory(ctx.device, vertexBufferMemory, nullptr);
}
