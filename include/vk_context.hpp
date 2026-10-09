#ifndef VK_CONTEXT_HPP

#define VK_CONTEXT_HPP

#include <vulkan/vulkan.h>

const int MAX_FRAMES_IN_FLIGHT = 2;

struct vk_context {
  VkInstance instance = VK_NULL_HANDLE;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device = VK_NULL_HANDLE;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  VkQueue graphicsQueue = VK_NULL_HANDLE;
  VkQueue presentQueue = VK_NULL_HANDLE;
};

// struct vk_context {
// };

#endif
