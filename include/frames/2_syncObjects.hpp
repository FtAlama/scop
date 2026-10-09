#ifndef SEMAPHORES_HPP

#define SEMAPHORES_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan_core.h>
#include <vector>

class SyncObjects {
public:
  SyncObjects(vk_context &, const std::vector<VkImageView> &);
	VkSemaphore availableSema() const { return imageAvailableSemaphore; }
	const std::vector<VkSemaphore> &renderFinishedSema() const { return renderFinishedSemaphores; }
	VkFence FlightFence() const { return inFlightFence; }
  ~SyncObjects();

private:
  vk_context &ctx;
 	VkSemaphore imageAvailableSemaphore;
 	std::vector<VkSemaphore> renderFinishedSemaphores;
 	VkFence inFlightFence;
};

#endif
