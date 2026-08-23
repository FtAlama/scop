#ifndef QUEUE_FAMILIES_HPP

#define QUEUE_FAMILIES_HPP

#include <cstdint>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &, VkSurfaceKHR &);

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &,
                                              VkSurfaceKHR &);

#endif
