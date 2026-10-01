#ifndef SEMAPHORES_HPP

#define SEMAPHORES_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan_core.h>

class SyncObjects {
public:
  SyncObjects(vk_context &);
  ~SyncObjects();

private:
  vk_context &ctx;
};

#endif
