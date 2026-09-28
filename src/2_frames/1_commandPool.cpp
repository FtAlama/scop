#include "frames/1_commandPool.hpp"
#include "queueFamilies.hpp"
#include "vk_context.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

CommandPool::CommandPool(vk_context &ctx) : ctx(ctx) {
  QueueFamilyIndices queueFamilyIndices =
      findQueueFamilies(ctx.physicalDevice, ctx.surface);
  VkCommandPoolCreateInfo poolInfo{};

  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(ctx.device, &poolInfo, nullptr, &ctx.commandPool) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create command pool!");
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(ctx.device, ctx.commandPool, nullptr);
}

CommandBuffer::CommandBuffer(vk_context &ctx) : ctx(ctx) {
  VkCommandBufferAllocateInfo allocInfo{};

  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = ctx.commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(ctx.device, &allocInfo, &ctx.commandBuffer) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to allocate command buffer!");
}

void CommandBuffer::recordCommandBuffer(uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};

  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;

  if (vkBeginCommandBuffer(ctx.commandBuffer, &beginInfo) != VK_SUCCESS)
    throw std::runtime_error("failed to begin commmand buffer!");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = ctx.renderPass;
  renderPassInfo.framebuffer = ctx.swapchainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = ctx.swapChainExtent;

  constexpr VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 0.1f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;
  vkCmdBeginRenderPass(ctx.commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(ctx.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    ctx.graphicsPipeline);
	vkCmdDraw(ctx.commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(ctx.commandBuffer);
	if (vkEndCommandBuffer(ctx.commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to record command buffer!");
}

CommandBuffer::~CommandBuffer() {}
