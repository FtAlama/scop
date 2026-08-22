#ifndef QUEUE_FAMILIES_HPP

#define QUEUE_FAMILIES_HPP

#include <cstdint>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct s_queueFamily {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
} QueueFamilyIndices;

typedef struct s_swapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
} SwapChainSupportDetails;

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &, VkSurfaceKHR &);

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &, VkSurfaceKHR &);



#endif
