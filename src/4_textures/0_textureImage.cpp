#include "texture_image/textureImage.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <glm/vector_relational.hpp>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

// #define STB_IMAGE_IMPLEMENTATION
#include "buffers/0_creationBuffers.hpp"
#include "stb_image.h"
#include "vk_context.hpp"

uint32_t static findImageMemoryType(uint32_t typeFilter,
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

StagingTexture::StagingTexture(vk_context &ctx, std::string const path)
    : ctx(ctx) {
  int texChannels;

  stbi_uc *pixels = stbi_load(path.c_str(), &coord.width, &coord.height,
                              &texChannels, STBI_rgb_alpha);
  if (!pixels)
    throw std::runtime_error("failed to open texture : " + path + " !");
  VkDeviceSize imageSize = coord.width * coord.height * 4;

  create_buffer(ctx, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);
  void *data;

  vkMapMemory(ctx.device, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(ctx.device, stagingBufferMemory);
  stbi_image_free(pixels);
}

inline void StagingTexture::destroyStaging() {
  vkDestroyBuffer(ctx.device, stagingBuffer, nullptr);
  vkFreeMemory(ctx.device, stagingBufferMemory, nullptr);
  isDestroy = true;
}

StagingTexture::~StagingTexture() {
  if (!isDestroy) {
    destroyStaging();
  }
}

Image::Image(vk_context &ctx, textCoord const &coord, VkFormat const format,
             VkImageTiling const tiling, VkImageUsageFlags const usage)
    : ctx(ctx) {
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = static_cast<uint32_t>(coord.width);
  imageInfo.extent.height = static_cast<uint32_t>(coord.height);
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;

  imageInfo.format = format;
  imageInfo.tiling = tiling;

  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = 0;

  if (vkCreateImage(ctx.device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    throw std::runtime_error("failed to create image!");
}

Image::~Image() { vkDestroyImage(ctx.device, image, nullptr); }

ImageDeviceMemory::ImageDeviceMemory(vk_context &ctx, VkImage const &image,
                                     VkMemoryPropertyFlags const properties)
    : ctx(ctx) {
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(ctx.device, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};

  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findImageMemoryType(memRequirements.memoryTypeBits, properties, ctx);
  if (vkAllocateMemory(ctx.device, &allocInfo, nullptr, &textureImageMemory) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to allocate image memory!");

  vkBindImageMemory(ctx.device, image, textureImageMemory, 0);
}

ImageDeviceMemory::~ImageDeviceMemory() {
  vkFreeMemory(ctx.device, textureImageMemory, nullptr);
}

TextureImage::TextureImage(vk_context &ctx, std::string const path,
                           VkFormat const format, VkImageTiling const tiling,
                           VkImageUsageFlags const usage,
                           VkMemoryPropertyFlags const properties)
    : ctx(ctx), stagingTexture(ctx, path),
      textureImage(ctx, stagingTexture.get_coord(), format, tiling, usage),
      textureImageMemory(ctx, textureImage.get(), properties) {}
