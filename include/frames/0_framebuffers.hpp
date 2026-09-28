#ifndef FRAMEBUFFERS_HPP

#define FRAMEBUFFERS_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan_core.h>

class Framebuffers {
public:
  Framebuffers(vk_context &);
  ~Framebuffers();

private:
  vk_context &ctx;
};

#endif
