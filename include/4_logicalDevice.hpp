#ifndef LOGICAL_DEVICE_HPP

#define LOGICAL_DEVICE_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class LogicalDevice {
public:
  LogicalDevice(vk_context &);
  void createLogicalDevice(VkPhysicalDevice &, VkSurfaceKHR &, VkQueue &,
                           VkQueue &);
  void destroyDevice();
  VkDevice &getDevice();
  ~LogicalDevice();

private:
  vk_context &ctx;
};

#endif
