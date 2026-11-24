#ifndef DESCRIPTORPOOL_HPP

#define DESCRIPTORPOOL_HPP

#include "vk_context.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

class DescriptorSetLayout {
public:
  DescriptorSetLayout(vk_context &);
  ~DescriptorSetLayout();
  const VkDescriptorSetLayout &get() const { return descriptorSetLayout; }

private:
  vk_context &ctx;
  VkDescriptorSetLayout descriptorSetLayout;
};

class DescriptorPool {
public:
	DescriptorPool(vk_context &, uint32_t const);
	~DescriptorPool();

private:
	vk_context &ctx;
};

#endif
