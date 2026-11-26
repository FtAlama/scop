#ifndef TEXTUREIMAGE_HPP

#define TEXTUREIMAGE_HPP

#include "vk_context.hpp"
#include <string>
#include <vulkan/vulkan_core.h>

struct textCoord {
  int width;
  int height;
};

class StagingTexture {
public:
  StagingTexture(vk_context &, std::string const);
  const textCoord &get_coord() const { return coord; }
  inline void destroyStaging();
  ~StagingTexture();

private:
  vk_context &ctx;
  textCoord coord;
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  bool isDestroy = false;
};

class Image {
public:
  Image(vk_context &, textCoord const &, VkFormat const, VkImageTiling const,
        VkImageUsageFlags const);
  ~Image();
  const VkImage &get() const { return image; }

private:
  vk_context &ctx;
  VkImage image;
};

class ImageDeviceMemory {
public:
  ImageDeviceMemory(vk_context &, VkImage const &, VkMemoryPropertyFlags const);
  ~ImageDeviceMemory();
  const VkDeviceMemory &get() const { return textureImageMemory; }

private:
  vk_context &ctx;
  VkDeviceMemory textureImageMemory;
};

class TextureImage {
public:
  TextureImage(vk_context &, std::string const, VkFormat const,
               VkImageTiling const, VkImageUsageFlags const,
               VkMemoryPropertyFlags const);
  const VkImage &get() const { return textureImage.get(); }
  ~TextureImage();

private:
  vk_context &ctx;
  StagingTexture stagingTexture;
  Image textureImage;
  ImageDeviceMemory textureImageMemory;
};

#endif
