#ifndef PHYSICAL_DEVICE_HPP

#define PHYSICAL_DEVICE_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

class PhysicalDevice {
public:
  PhysicalDevice(vk_context &);

private:
  int rateDeviceSuitable(const VkPhysicalDevice &, VkSurfaceKHR &);
  bool checkDeviceExtensionSupport(const VkPhysicalDevice &);
};

#endif
