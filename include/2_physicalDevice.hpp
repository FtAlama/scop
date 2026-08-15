#ifndef PHYSICAL_DEVICE_HPP

#define PHYSICAL_DEVICE_HPP

#include "queueFamilies.hpp"
#include <vulkan/vulkan_core.h>

class PhysicalDevice {
public:
  void pickPhysicalDevice(VkInstance &);
  VkDevice &getDevice();
	VkPhysicalDevice &getPhysicalDevice();
	~PhysicalDevice();

private:
  VkDevice device;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  int rateDeviceSuitable(VkPhysicalDevice);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};

#endif
