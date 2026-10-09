#ifndef GRAPHICS_PIPELINE_HPP

#define GRAPHICS_PIPELINE_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class GraphicsPipeline {
public:
  GraphicsPipeline(vk_context &, VkRenderPass const);
  VkPipeline get() const { return graphicsPipeline; }
	VkPipelineLayout pipelineLay() const { return pipelineLayout; }
  ~GraphicsPipeline();

private:
  VkShaderModule createShaderModule(const std::vector<char> &);
  vk_context &ctx;
  VkPipelineShaderStageCreateInfo shaderStages[2];
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  VkPipelineInputAssemblyStateCreateInfo inputAssambly{};
  VkPipelineViewportStateCreateInfo vpState{};
  VkViewport viewport{};
  VkRect2D scissor{};
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  VkPipelineMultisampleStateCreateInfo multisampling{};
  VkPipelineColorBlendStateCreateInfo colorBlending{};
  std::vector<VkDynamicState> dynamicStates;
  VkPipelineDynamicStateCreateInfo dynamicState{};
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  VkPipeline graphicsPipeline = VK_NULL_HANDLE;
  VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
  void vertexInput();
  void inputAsm();
  void viewportState();
  void set_rasterization();
  void multisample();
  void colorBlend();
  void dynamicSte();
  void createGraphicsPipeline(VkRenderPass const);
};

#endif
