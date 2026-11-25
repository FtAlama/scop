#include "buffers/1_uniformBuffers.hpp"
#include "buffers/2_descriptorPool.hpp"
#include "vk_context.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

DescriptorSetLayout::DescriptorSetLayout(vk_context &ctx) : ctx(ctx) {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutCreateInfo layoutInfo{};

  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = 1;
  layoutInfo.pBindings = &uboLayoutBinding;

  if (vkCreateDescriptorSetLayout(ctx.device, &layoutInfo, nullptr,
                                  &descriptorSetLayout) != VK_SUCCESS)
    throw std::runtime_error("failed to create descriptor set layout!");
}

DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(ctx.device, descriptorSetLayout, nullptr);
}

DescriptorPool::DescriptorPool(vk_context &ctx, uint32_t const frame_size)
    : ctx(ctx) {
  VkDescriptorPoolSize poolSize{};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = static_cast<uint32_t>(frame_size);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.maxSets = static_cast<uint32_t>(frame_size);
  poolInfo.flags = 0;

  if (vkCreateDescriptorPool(ctx.device, &poolInfo, nullptr, &descriptorPool) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create descriptor pool!");
}

DescriptorPool::~DescriptorPool() {
  vkDestroyDescriptorPool(ctx.device, descriptorPool, nullptr);
}

DescriptorSets::DescriptorSets(vk_context &ctx,
                               VkDescriptorSetLayout const descriptorSetLayout,
                               uint32_t const frames_size,
                               VkDescriptorPool const descriptorPool,
                               std::vector<VkBuffer> const &uniformBuffers)
    : ctx(ctx) {
  std::vector<VkDescriptorSetLayout> layouts(frames_size, descriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.descriptorSetCount = static_cast<uint32_t>(frames_size);
  allocInfo.pSetLayouts = layouts.data();

  descriptorSets.resize(frames_size);
  if (vkAllocateDescriptorSets(ctx.device, &allocInfo, descriptorSets.data()) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create descriptor sets!");

  for (uint32_t i = 0; i < frames_size; i++) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSets[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(ctx.device, 1, &descriptorWrite, 0, nullptr);
  }
}

DescriptorSets::~DescriptorSets() {}
