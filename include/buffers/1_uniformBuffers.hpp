#ifndef UNIFORM_BUFFERS_HPP

#define UNIFORM_BUFFERS_HPP

#include "vk_context.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

class UniformBuffers{
public:
	UniformBuffers(vk_context &, uint32_t);
	~UniformBuffers();
	void updateUniformBuffer(uint32_t, VkExtent2D const);

private:
	vk_context &ctx;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void *> uniformBuffersMapped;
	uint32_t frames_size;
};


#endif
