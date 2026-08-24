#include "pipeline/0_graphics_pipeline.hpp"
#include "vk_context.hpp"
#include <cstdint>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (!file.is_open())
    throw std::runtime_error(std::string{"failed to open file : " + filename});

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();
  return (buffer);
}

GraphicsPipeline::GraphicsPipeline(vk_context &ctx) : ctx(ctx) {
  auto vertShaderCode = readFile("shaders/shader.vert");
  auto fragShaderCode = readFile("shaders/shader.frag");

  vertShaderModule = createShaderModule(vertShaderCode);
  fragShaderModule = createShaderModule(fragShaderCode);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";
  shaderStages[0] = vertShaderStageInfo;
  shaderStages[1] = fragShaderStageInfo;
  vkDestroyShaderModule(ctx.device, fragShaderModule, nullptr);
  vkDestroyShaderModule(ctx.device, vertShaderModule, nullptr);
}

VkShaderModule
GraphicsPipeline::createShaderModule(const std::vector<char> &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
  VkShaderModule shaderModule;
  if (vkCreateShaderModule(ctx.device, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS)
    throw std::runtime_error("failed to create shader module!");
  return (shaderModule);
}

GraphicsPipeline::~GraphicsPipeline() {}
