#ifndef QUEUE_FAMILIES_HPP

#define QUEUE_FAMILIES_HPP

#include <cstdint>
#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct s_queueFamily {
  std::optional<uint32_t> graphicsFamily;
  bool isComplete() { return graphicsFamily.has_value(); }
} QueueFamilyIndices;

class QueueFamilies {
public:
  static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
	VkQueue &getGraphicsQueue();
private:
	VkQueue graphicsQueue;
};

#endif
