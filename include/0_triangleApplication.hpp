#ifndef TRIANGLE_APPLICATION_HPP

#define TRIANGLE_APPLICATION_HPP

#include "1_validationLayers.hpp"
#include "2_physicalDevice.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class TriangleApplication {
public:
  TriangleApplication();
  ~TriangleApplication();

  void run() {
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;
	PhysicalDevice vk_device;
	ValidationLayers layers;

  void initVulkan();
  void mainLoop();
  void cleanup();
  void createInstance();

  void createLogicalDevice();
};

#endif
