#ifndef IMAGE_VIEWS_HPP

#define IMAGE_VIEWS_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

class ImageViews {
public:
  ImageViews(vk_context &, const std::vector<VkImage> &, const VkFormat);
  void createImageViews(const std::vector<VkImage> &, const VkFormat);
  void destroyImageViews(VkDevice &);
  std::vector<VkImageView> get() const { return swapChainImageViews; }
  void cleanup();
  ~ImageViews();

private:
  std::vector<VkImageView> swapChainImageViews;
  vk_context &ctx;
};

#endif
