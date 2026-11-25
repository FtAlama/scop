#ifndef DESCRIPTORPOOL_HPP

#define DESCRIPTORPOOL_HPP

#include "vk_context.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
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
  const VkDescriptorPool &get() const { return descriptorPool; }

private:
  vk_context &ctx;
  VkDescriptorPool descriptorPool;
};

class DescriptorSets {
public:
  DescriptorSets(vk_context &, VkDescriptorSetLayout const, uint32_t const,
                 VkDescriptorPool const, std::vector<VkBuffer> const &);
  ~DescriptorSets();
  const std::vector<VkDescriptorSet> &get() const { return descriptorSets; }

private:
  vk_context &ctx;
  std::vector<VkDescriptorSet> descriptorSets;
};

#endif
