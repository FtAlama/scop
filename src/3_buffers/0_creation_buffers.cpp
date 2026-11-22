#include "buffers/0_creation_buffers.hpp"
#include <cstring>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

uint32_t static findMemoryType(uint32_t typeFilter,
                               const VkMemoryPropertyFlags properties,
                               VkBuffer &buffer, vk_context &ctx) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(ctx.physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }
  vkDestroyBuffer(ctx.device, buffer, nullptr);
  throw std::runtime_error("failed to find suitable memory type!");
}

void create_buffer(vk_context &ctx, VkDeviceSize size, VkBufferUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkBuffer &buffer,
                   VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(ctx.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    throw std::runtime_error("failed to create vertex buffer!");

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(ctx.device, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties, buffer, ctx);
  if (vkAllocateMemory(ctx.device, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    vkDestroyBuffer(ctx.device, buffer, nullptr);
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  }
  vkBindBufferMemory(ctx.device, buffer, bufferMemory, 0);
}
