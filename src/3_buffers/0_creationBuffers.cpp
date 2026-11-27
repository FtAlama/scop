#include "buffers/0_creationBuffers.hpp"
#include "vk_context.hpp"
#include <cstring>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

StagingBuffer::StagingBuffer(vk_context &ctx)
    : ctx(ctx), stagingBufferMemory(ctx), stagingBuffer(ctx) {}

void StagingBuffer::create(VkDeviceSize const size,
                           VkBufferUsageFlags const usage,
                           VkMemoryPropertyFlags const properties) {
  if (isCreated)
    throw std::runtime_error("staging buffer (main) already created!");
  isCreated = true;

  stagingBuffer.create(size, usage);
  stagingBufferMemory.create(stagingBuffer.get(), properties);
}

void StagingBuffer::createImage(std::string const path) {
  int texChannels;
  if (isCreated)
    throw std::runtime_error("staging buffer already created!");
  isCreated = true;

  stbi_uc *pixels = stbi_load(path.c_str(), &coord.width, &coord.height,
                              &texChannels, STBI_rgb_alpha);
  if (!pixels)
    throw std::runtime_error("failed to open texture : " + path + " !");
  VkDeviceSize imageSize = coord.width * coord.height * 4;

  stagingBuffer.create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  stagingBufferMemory.create(stagingBuffer.get(),
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  void *data;

  vkMapMemory(ctx.device, stagingBufferMemory.get(), 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(ctx.device, stagingBufferMemory.get());
  stbi_image_free(pixels);
}

inline void StagingBuffer::destroyStaging() {
  stagingBufferMemory.deleteBufferMemory();
  stagingBuffer.deleteBuffer();
  isDestroy = true;
}

StagingBuffer::~StagingBuffer() {
  if (!isDestroy) {
    destroyStaging();
  }
}

Buffer::Buffer(vk_context &ctx) : ctx(ctx) {
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

void Buffer::create(VkDeviceSize const size, VkBufferUsageFlags const usage) {
  bufferInfo.size = size;
  bufferInfo.usage = usage;

  if (vkCreateBuffer(ctx.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    throw std::runtime_error("failed to create vertex buffer!");
}

void Buffer::deleteBuffer() {
  vkDestroyBuffer(ctx.device, buffer, nullptr);
  isDestroy = true;
  buffer = VK_NULL_HANDLE;
}

Buffer::~Buffer() {
  if (!isDestroy) {
    vkDestroyBuffer(ctx.device, buffer, nullptr);
  }
}

BufferMemory::BufferMemory(vk_context &ctx) : ctx(ctx) {}

uint32_t findMemoryType(uint32_t typeFilter,
                        const VkMemoryPropertyFlags properties,
                        vk_context &ctx) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(ctx.physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }
  throw std::runtime_error("failed to find suitable memory type!");
}

void BufferMemory::create(VkBuffer const &buffer,
                          VkMemoryPropertyFlags const properties) {
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(ctx.device, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties, ctx);
  if (vkAllocateMemory(ctx.device, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  vkBindBufferMemory(ctx.device, buffer, bufferMemory, 0);
}

void BufferMemory::deleteBufferMemory() {
  vkFreeMemory(ctx.device, bufferMemory, nullptr);
  bufferMemory = VK_NULL_HANDLE;
  isDestroy = true;
}

BufferMemory::~BufferMemory() {
  if (!isDestroy) {
    vkFreeMemory(ctx.device, bufferMemory, nullptr);
  }
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
                vk_context &ctx) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = ctx.commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(ctx.device, &allocInfo, &commandBuffer);
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;

  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(ctx.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(ctx.graphicsQueue);
  vkFreeCommandBuffers(ctx.device, ctx.commandPool, 1, &commandBuffer);
}
