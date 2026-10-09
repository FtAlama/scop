#ifndef IMAGE_VIEWS_HPP

#define IMAGE_VIEWS_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

class ImageViews {
public:
  ImageViews(vk_context &, std::vector<VkImage> const &, VkFormat const);
  void createImageViews(std::vector<VkImage> &, VkFormat &, VkDevice &);
  void destroyImageViews(VkDevice &);
  std::vector<VkImageView> get() const { return swapChainImageViews; }
  ~ImageViews();

private:
  std::vector<VkImageView> swapChainImageViews;
  vk_context &ctx;
};

#endif
