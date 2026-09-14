#ifndef GRAPHICS_PIPELINE_HPP

#define GRAPHICS_PIPELINE_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class GraphicsPipeline {
public:
  GraphicsPipeline(vk_context &);
  ~GraphicsPipeline();

private:
  VkShaderModule createShaderModule(const std::vector<char> &);
  VkPipeline graphicsPipeline = VK_NULL_HANDLE;
  vk_context &ctx;
  VkPipelineShaderStageCreateInfo shaderStages[2];
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  VkPipelineInputAssemblyStateCreateInfo inputAssambly{};
  VkPipelineViewportStateCreateInfo vpState{};
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  VkPipelineMultisampleStateCreateInfo multisampling{};
  VkPipelineColorBlendStateCreateInfo colorBlending{};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  void vertexInput();
  void inputAsm();
  void viewportState();
  void set_rasterization();
  void multisample();
  void colorBlend();
  void dynamicSte();
  void createGraphicsPipeline();
};

#endif
