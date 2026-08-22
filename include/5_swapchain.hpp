#ifndef SWAP_CHAIN_HPP

#define SWAP_CHAIN_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class SwapChain {
public:
  void CreateSwapChain(const VkPhysicalDevice &device, const VkDevice &,
                       VkSurfaceKHR &surface, GLFWwindow *);
  VkSwapchainKHR &getSwapChain();
  void destroySwapChain(VkDevice &);

private:
  VkSurfaceFormatKHR
  chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &, GLFWwindow *);
  VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
};

#endif
