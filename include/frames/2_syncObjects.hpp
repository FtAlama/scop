#ifndef SEMAPHORES_HPP

#define SEMAPHORES_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

class SyncObjects {
public:
  SyncObjects(vk_context &, const uint32_t);
  const std::vector<frame_flight> &framesFlight() const { return frames; }
  const std::vector<VkSemaphore> renderFinished() const {
    return renderFinishedSemaphores;
  }
  void recreateRenderFinished(const uint32_t);
  ~SyncObjects();

private:
  vk_context &ctx;
  std::vector<frame_flight> frames;
  std::vector<VkSemaphore> renderFinishedSemaphores;
};

#endif
