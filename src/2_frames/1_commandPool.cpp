#include "frames/1_commandPool.hpp"
#include "queueFamilies.hpp"
#include "vertex.hpp"
#include "vk_context.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

CommandPool::CommandPool(vk_context &ctx) : ctx(ctx) {
  QueueFamilyIndices queueFamilyIndices =
      findQueueFamilies(ctx.physicalDevice, ctx.surface);
  VkCommandPoolCreateInfo poolInfo{};

  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(ctx.device, &poolInfo, nullptr, &commandPool) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create command pool!");
  ctx.commandPool = commandPool;
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(ctx.device, commandPool, nullptr);
}

CommandBuffer::CommandBuffer(vk_context &ctx, const VkCommandPool commandPool)
    : ctx(ctx) {
  commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocInfo{};

  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

  if (vkAllocateCommandBuffers(this->ctx.device, &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS)
    throw std::runtime_error("failed to allocate command buffer!");
}

void CommandBuffer::recordCommandBuffer(
    const VkCommandBuffer commandBuffer,
    const VkFramebuffer swapchainFramebuffer, const VkRenderPass renderPass,
    const VkExtent2D swapChainExtent, const VkPipeline graphicsPipeline,
    const VkBuffer &meshBuffer,
    const std::vector<VkDescriptorSet> &descriptorSets,
    VkPipelineLayout const pipelineLayout, uint32_t current_frame) {
  VkCommandBufferBeginInfo beginInfo{};

  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;
  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    throw std::runtime_error("failed to begin commmand buffer!");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = swapchainFramebuffer;
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChainExtent;

  constexpr VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 0.1f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;
  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChainExtent.width);
  viewport.height = static_cast<float>(swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChainExtent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  VkBuffer meshBuffers[] = {meshBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, meshBuffers, offsets);
  VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();
  vkCmdBindIndexBuffer(commandBuffer, meshBuffer, vertexSize,
                       VK_INDEX_TYPE_UINT16);
  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipelineLayout, 0, 1, &descriptorSets[current_frame],
                          0, nullptr);

  vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0,
                   0, 0);
  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    throw std::runtime_error("failed to record command buffer!");
}

CommandBuffer::~CommandBuffer() {}
