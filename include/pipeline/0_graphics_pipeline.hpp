#ifndef GRAPHICS_PIPELINE_HPP

#define GRAPHICS_PIPELINE_HPP

#include "vk_context.hpp"
#include <vector>
#include <vulkan/vulkan.h>

class GraphicsPipeline {
public:
  GraphicsPipeline(vk_context &);
  ~GraphicsPipeline();

private:
  VkShaderModule createShaderModule(const std::vector<char> &);
  vk_context &ctx;
  VkPipelineShaderStageCreateInfo shaderStages[2];
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
	VkPipelineLayout pipelineLayout;
};

#endif
