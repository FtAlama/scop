#ifndef LOGICAL_DEVICE_HPP

#define LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class LogicalDevice {
public:
  void createLogicalDevice(VkPhysicalDevice &, VkSurfaceKHR &, VkQueue &,
                           VkQueue &);
  void destroyDevice();
	VkDevice &getDevice();
private:
  VkDevice device;
};

#endif
