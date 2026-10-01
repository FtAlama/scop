#ifndef TRIANGLE_APPLICATION_HPP

#define TRIANGLE_APPLICATION_HPP

#include "1_validationLayers.hpp"
#include "2_windowSurface.hpp"
#include "3_physicalDevice.hpp"
#include "4_logicalDevice.hpp"
#include "6_imageViews.hpp"
#include "frames/2_syncObjects.hpp"
#include "vk_context.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "5_swapChain.hpp"
#include "frames/0_framebuffers.hpp"
#include "frames/1_commandPool.hpp"
#include "pipeline/0_renderPass.hpp"
#include "pipeline/1_graphicsPipeline.hpp"
#include <GLFW/glfw3.h>
#include <optional>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Instance {
public:
  Instance(vk_context &, ValidationLayers &);
  ~Instance();

private:
  vk_context &ctx;
};

class GlfwWindow {
public:
  GlfwWindow(uint32_t width, uint32_t height, const char *title) {
    glfwSetErrorCallback([](int code, const char *desc) {
      std::cerr << "GLFW error " << code << ": " << desc << std::endl;
    });
    if (!glfwInit())
      throw std::runtime_error("failed to init GLFW");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!handle)
      throw std::runtime_error("failed to create GLFW window");
  }
  ~GlfwWindow() {
    glfwDestroyWindow(handle);
    glfwTerminate();
  }
  GLFWwindow *get() { return handle; }
  bool shouldClose() { return glfwWindowShouldClose(handle); }

private:
  GLFWwindow *handle = nullptr;
};

class TriangleApplication {
public:
  void run() {
    initVulkan();
    mainLoop();
  }

private:
  std::optional<GlfwWindow> window;
  vk_context ctx;
  std::optional<Instance> instance;
  ValidationLayers layers;
  std::optional<WindowSurface> win_surface;
  std::optional<PhysicalDevice> vk_Pdevice;
  std::optional<LogicalDevice> vk_Ldevice;
  std::optional<SwapChain> swapchain;
  std::optional<ImageViews> imageView;
  std::optional<RenderPass> renderPass;
  std::optional<GraphicsPipeline> graphicsPipeline;
  std::optional<Framebuffers> framebuffers;
  std::optional<CommandPool> commandPool;
  std::optional<CommandBuffer> commandBuffer;
  std::optional<SyncObjects> syncObjects;

  void initVulkan();
  void mainLoop();
  void createInstance();
  void createLogicalDevice();
  void drawFrame();
};

#endif
