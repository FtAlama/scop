#include "texture_image/textureImage.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "buffers/0_creationBuffers.hpp"
#include "stb_image.h"
#include "vk_context.hpp"

TextureImage::TextureImage(vk_context &ctx, std::string const path) : ctx(ctx) {
  int texWidth, texHeight, texChannels;

  stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels,
                             STBI_rgb_alpha);
  if (!pixels)
    throw std::runtime_error("failed to open texture : " + path + " !");
  VkDeviceSize imageSize = texHeight * texWidth * 4;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  create_buffer(ctx, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);
  void *data;

  vkMapMemory(ctx.device, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(ctx.device, stagingBufferMemory);
  stbi_image_free(pixels);

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(texWidth);
	imageInfo.extent.height = static_cast<uint32_t>(texHeight);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;

	imageInfo.format = VK_FORMAT_R8G8B8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	if (vkCreateImage(ctx.device, &imageInfo, nullptr, &textureImage) != VK_SUCCESS)
		throw std::runtime_error("failed to create image!");


}

TextureImage::~TextureImage() {
	vkDestroyImage(ctx.device, textureImage, nullptr);
	vkFreeMemory(ctx.device, textureImageMemory, nullptr);
}
