#ifndef RENDERPASS_HPP

#define RENDERPASS_HPP

#include "vk_context.hpp"

class RenderPass {
public:
  RenderPass(vk_context &, VkFormat const);
  VkRenderPass get() const { return renderPass; }
  ~RenderPass();

private:
  VkRenderPass renderPass = VK_NULL_HANDLE;
  vk_context &ctx;
};

#endif
