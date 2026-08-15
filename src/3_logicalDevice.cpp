#include "3_logicalDevice.hpp"
#include "1_validationLayers.hpp"
#include "queueFamilies.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

void LogicalDevice::createLogicalDevice(VkPhysicalDevice &physicalDevice) {
  QueueFamilyIndices indices = QueueFamilies::findQueueFamilies(physicalDevice);

  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
  queueCreateInfo.queueCount = 1;

  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;
  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = 0;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else
    createInfo.enabledLayerCount = 0;

  if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device");
  }
	QueueFamilies queue;
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &queue.getGraphicsQueue());
	std::cout << "Logical device & queue familie create\n";
}

VkQueue &QueueFamilies::getGraphicsQueue() { return graphicsQueue; }

void LogicalDevice::destroyDevice() { vkDestroyDevice(device, nullptr); }
