#include "2_physicalDevice.hpp"
#include "queueFamilies.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>

void PhysicalDevice::pickPhysicalDevice(VkInstance &instance,
                                        VkSurfaceKHR &surface) {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0)
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
  std::multimap<int, VkPhysicalDevice> candidates;
  for (const auto &device : devices) {
    int score = rateDeviceSuitable(device, surface);
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

int PhysicalDevice::rateDeviceSuitable(const VkPhysicalDevice &device,
                                       VkSurfaceKHR &surface) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  QueueFamilyIndices indices = findQueueFamilies(device, surface);
  bool extensionSupported = checkDeviceExtensionSupport(device);

  if (!extensionSupported) {
    std::cerr << "one device does not support all the required extension\n";
    return (0);
  }
  bool swapChainAdequate = false;
  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(device, surface);
  swapChainAdequate = !swapChainSupport.formats.empty() &&
                      !swapChainSupport.presentModes.empty();
  if (!swapChainAdequate) {
    std::cerr << "Swapchain can't be initialize in a certains device\n";
    return (0);
  }

  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  std::cout << deviceProperties.deviceName << std::endl;
  int score = 0;
  if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;
  score += deviceProperties.limits.maxImageDimension2D;
  if (!deviceFeatures.geometryShader || !indices.isComplete()) {
    return (0);
  }
  return (score);
}

bool PhysicalDevice::checkDeviceExtensionSupport(
    const VkPhysicalDevice &device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());
  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());
  for (const auto &extension : availableExtensions)
    requiredExtensions.erase(extension.extensionName);
  return requiredExtensions.empty();
}

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &device,
                                     VkSurfaceKHR &surface) {
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;
  VkBool32 presentSupport = false;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());
  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;
    presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport)
      indices.presentFamily = i;
    if (indices.isComplete())
      break;
    i++;
  }
  return (indices);
}

VkPhysicalDevice &PhysicalDevice::getPhysicalDevice() { return physicalDevice; }
