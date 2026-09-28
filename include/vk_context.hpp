#ifndef VK_CONTEXT_HPP

#define VK_CONTEXT_HPP

#include <vulkan/vulkan.h>
#include <vector>

struct vk_context {
  VkInstance instance = VK_NULL_HANDLE;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device = VK_NULL_HANDLE;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  VkQueue graphicsQueue = VK_NULL_HANDLE;
  VkQueue presentQueue = VK_NULL_HANDLE;
  VkRenderPass renderPass = VK_NULL_HANDLE;
  VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
  VkExtent2D swapChainExtent;
  VkFormat swapChainImageFormat;
	std::vector<VkImageView> swapChainImageViews;
	VkCommandPool commandPool = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> swapchainFramebuffers;
  VkPipeline graphicsPipeline = VK_NULL_HANDLE;
  VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
};

#endif
