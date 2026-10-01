#ifndef SWAP_CHAIN_HPP

#define SWAP_CHAIN_HPP

#include "vk_context.hpp"
#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class SwapChain {
public:
  SwapChain(vk_context &, GLFWwindow *);
  VkSwapchainKHR &getSwapChain();
  std::vector<VkImage> &getChainImage();
  VkFormat &getSwapChainImageFormat();
  VkExtent2D &getExtent();
  ~SwapChain();

private:
  VkSurfaceFormatKHR
  chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &, GLFWwindow *);
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  vk_context &ctx;
};

#endif
