#include "setup/2_windowSurface.hpp"
#include "queueFamilies.hpp"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <vulkan/vulkan_core.h>

WindowSurface::WindowSurface(vk_context &ctx, GLFWwindow *window)
    : ctx(ctx) {
  if (glfwCreateWindowSurface(ctx.instance, window, nullptr,
                              &ctx.surface) != VK_SUCCESS)
    throw std::runtime_error("failed to create window surface");
  std::cout << "Window surface create\n";
}

WindowSurface::~WindowSurface() {
  vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
}

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device,
                                              VkSurfaceKHR &surface) {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }
  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }
  return (details);
}
