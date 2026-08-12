#include "queueFamilies.hpp"
#include "triangleApplication.hpp"
#include <cstdint>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>

void TriangleApplication::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (deviceCount == 0)
    throw std::runtime_error("failed to find GPUs with Vulkan support!");

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  std::multimap<int, VkPhysicalDevice> candidates;

  for (const auto &device : devices) {
    int score = rateDeviceSuitable(device);
    candidates.insert(std::make_pair(score, device));
 }
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(candidates.rbegin()->second, &deviceProperties);

	std::cout << "device pick : " << deviceProperties.deviceName << std::endl;
  if (candidates.rbegin()->first > 0)
    physicalDevice = candidates.rbegin()->second;
  else
    throw std::runtime_error("failed to find a suitable GPU!");
}

int TriangleApplication::rateDeviceSuitable(VkPhysicalDevice device) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;

  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	std::cout << deviceProperties.deviceName << std::endl;
  int score = 0;
  if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;
  score += deviceProperties.limits.maxImageDimension2D;
  if (!deviceFeatures.geometryShader) {
    return (0);
  }
  return (score);
}

QueueFamilyIndices
TriangleApplication::findQueueFamilies(VkPhysicalDevice device) {
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());
  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;
    i++;
  }
  return (indices);
}
