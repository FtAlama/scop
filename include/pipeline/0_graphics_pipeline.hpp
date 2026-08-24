#ifndef GRAPHICS_PIPELINE_HPP

#define GRAPHICS_PIPELINE_HPP

#include "vk_context.hpp"
#include <vulkan/vulkan.h>
#include <vector>

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
};

#endif
