#ifndef UNIFORM_BUFFERS_HPP

#define UNIFORM_BUFFERS_HPP

#include "buffers/0_creationBuffers.hpp"
#include "vk_context.hpp"
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

class UniformBuffers {
public:
  UniformBuffers(vk_context &, uint32_t);
  ~UniformBuffers();
  void updateUniformBuffer(uint32_t, VkExtent2D const);
  std::vector<VkBuffer> &get() {
    for (uint32_t i = 0; i < uniformBuffers.size(); i++) {
      sendBuffer.push_back(uniformBuffers[i].get());
    }
    return sendBuffer;
  }

private:
  vk_context &ctx;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;

  std::vector<BufferMemory> uniformBuffersMemory;
  std::vector<Buffer> uniformBuffers;
  std::vector<VkBuffer> sendBuffer;
  std::vector<void *> uniformBuffersMapped;
  uint32_t frames_size;
};

#endif
