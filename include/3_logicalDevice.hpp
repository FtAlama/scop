#ifndef LOGICAL_DEVICE_HPP

#define LOGICAL_DEVICE_HPP

#include <vulkan/vulkan_core.h>

class LogicalDevice {
public:
	void createLogicalDevice(VkPhysicalDevice &);
	void destroyDevice();

private:
  VkDevice device;
};

#endif
