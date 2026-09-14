#include "setup/5_swapChain.hpp"
#include "queueFamilies.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

SwapChain::SwapChain(vk_context &ctx, GLFWwindow *window) : ctx(ctx) {
  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(ctx.physicalDevice, ctx.surface);
  VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);
  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount)
    imageCount = swapChainSupport.capabilities.maxImageCount;
  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = ctx.surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  QueueFamilyIndices indices =
      findQueueFamilies(ctx.physicalDevice, ctx.surface);
  uint32_t queueFamiliesIndices[] = {indices.graphicsFamily.value(),
                                     indices.presentFamily.value()};
  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamiliesIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;
  if (vkCreateSwapchainKHR(ctx.device, &createInfo, nullptr, &swapChain) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create swap chain!");
  vkGetSwapchainImagesKHR(ctx.device, swapChain, &imageCount, nullptr);
  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(ctx.device, swapChain, &imageCount,
                          swapChainImages.data());

  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;
  ctx.swapChainExtent = swapChainExtent;
  ctx.swapChainImageFormat = swapChainImageFormat;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return (availableFormat);
  }
  return (availableFormats[0]);
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      std::cout << "SwapChain MAILBOX mode present\n";
      return (availablePresentMode);
    }
  }
  std::cout << "FIFO mode for swap chain enable\n";
  return (VK_PRESENT_MODE_FIFO_KHR);
}

VkExtent2D
SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            GLFWwindow *window) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    return (capabilities.currentExtent);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                             static_cast<uint32_t>(height)};
  actualExtent.width =
      std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                 capabilities.maxImageExtent.width);
  actualExtent.height =
      std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                 capabilities.maxImageExtent.height);
  return actualExtent;
}

VkSwapchainKHR &SwapChain::getSwapChain() { return (swapChain); }

std::vector<VkImage> &SwapChain::getChainImage() { return (swapChainImages); }

VkFormat &SwapChain::getSwapChainImageFormat() {
  return (swapChainImageFormat);
}

SwapChain::~SwapChain() {
  vkDestroySwapchainKHR(ctx.device, swapChain, nullptr);
}
