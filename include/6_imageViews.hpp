#ifndef IMAGE_VIEWS_HPP

#define IMAGE_VIEWS_HPP


#include <vector>
#include <vulkan/vulkan_core.h>

class ImageViews {
public:
	void createImageViews(std::vector<VkImage> &, VkFormat &, VkDevice &);
	void destroyImageViews(VkDevice &);
private:
	std::vector<VkImageView> swapChainImageViews;
};

#endif
