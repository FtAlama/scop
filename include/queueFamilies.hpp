#ifndef QUEUE_FAMILIES_HPP

#define QUEUE_FAMILIES_HPP

#include <cstdint>
#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct s_queueFamily {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
} QueueFamilyIndices;

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &, VkSurfaceKHR &);

#endif
