#ifndef LOGICAL_DEVICE_HPP

#define LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>

class LogicalDevice {
public:
	void createLogicalDevice(VkPhysicalDevice &, VkSurfaceKHR &, VkQueue &, VkQueue &);
	void destroyDevice();

private:
  VkDevice device;
};

#endif
