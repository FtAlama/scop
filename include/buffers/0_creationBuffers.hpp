#ifndef CREATION_BUFFERS_HPP

#define CREATION_BUFFERS_HPP

#include "vk_context.hpp"
#include <string>
#include <vulkan/vulkan_core.h>

struct textCoord {
  int width;
  int height;
};

class Buffer {
public:
  Buffer(vk_context &);
  Buffer(Buffer &&copy) noexcept
      : ctx(copy.ctx), bufferInfo(copy.bufferInfo), isDestroy(copy.isDestroy) {
    copy.buffer = VK_NULL_HANDLE;
    copy.isDestroy = true;
  }
  ~Buffer();

  VkBuffer &get() { return buffer; }
  void create(VkDeviceSize const, VkBufferUsageFlags const);
  void deleteBuffer();

private:
  vk_context &ctx;
  VkBuffer buffer = VK_NULL_HANDLE;
  VkBufferCreateInfo bufferInfo{};
  bool isDestroy = false;
};

class BufferMemory {
public:
  BufferMemory(vk_context &);
  BufferMemory(BufferMemory &&o) noexcept
      : ctx(o.ctx), bufferMemory(o.bufferMemory), isDestroy(o.isDestroy) {
    o.bufferMemory = VK_NULL_HANDLE;
    o.isDestroy = true;
  }
  ~BufferMemory();
  VkDeviceMemory &get() { return bufferMemory; }
  void deleteBufferMemory();
  void create(VkBuffer const &, VkMemoryPropertyFlags const);

private:
  vk_context &ctx;
  VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
  bool isDestroy = false;
};

uint32_t findMemoryType(uint32_t, const VkMemoryPropertyFlags, vk_context &ctx);

class StagingBuffer {
public:
  StagingBuffer(vk_context &);
  void create(VkDeviceSize const, VkBufferUsageFlags const,
              VkMemoryPropertyFlags const);
  void createImage(std::string const);
  const textCoord &get_coord() const { return coord; }
  void destroyStaging();
  VkBuffer &get() { return (stagingBuffer.get()); }
  VkDeviceMemory &getMemory() { return stagingBufferMemory.get(); }
  ~StagingBuffer();

private:
  vk_context &ctx;
  textCoord coord;
  BufferMemory stagingBufferMemory;
  Buffer stagingBuffer;
  bool isDestroy = false;
  bool isCreated = false;
};

class MeshBuffer {
public:
  MeshBuffer(vk_context &);
  ~MeshBuffer();
  VkBuffer &get() { return meshBuffer.get(); }

private:
  vk_context &ctx;
  BufferMemory meshBufferMemory;
  Buffer meshBuffer;
};

void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize, vk_context &);

#endif
