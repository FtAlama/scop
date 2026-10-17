#ifndef SEMAPHORES_HPP

#define SEMAPHORES_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

class SyncObjects {
public:
  SyncObjects(vk_context &, const uint32_t);
  const std::vector<frame_flight> &framesFlight() const { return frames; }
  void recreateRenderFinished();
  ~SyncObjects();
  VkSemaphore renderFinished(uint32_t i) const {
    return (renderFinishedSemaphores[i]);
  };

private:
  vk_context &ctx;
  std::vector<frame_flight> frames;
  std::vector<VkSemaphore> renderFinishedSemaphores;
};

#endif
