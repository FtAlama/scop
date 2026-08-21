#ifndef PHYSICAL_DEVICE_HPP

#define PHYSICAL_DEVICE_HPP

#include <vulkan/vulkan_core.h>

class PhysicalDevice {
public:
  void pickPhysicalDevice(VkInstance &, VkSurfaceKHR &);
	VkPhysicalDevice &getPhysicalDevice();
	~PhysicalDevice();

private:
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  int rateDeviceSuitable(const VkPhysicalDevice &, VkSurfaceKHR &);
};

#endif
