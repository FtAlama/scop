#ifndef TRIANGLE_APPLICATION_HPP

#define TRIANGLE_APPLICATION_HPP

#include "1_validationLayers.hpp"
#include "2_physicalDevice.hpp"
#include "3_logicalDevice.hpp"
#include "4_window_surface.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "5_swapchain.hpp"

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
	ValidationLayers layers;
	PhysicalDevice vk_Pdevice;
	LogicalDevice vk_Ldevice;
	WindowSurface win_surface;
  VkQueue graphicsQueue;
	SwapChain swapchain;
	VkQueue presentQueue;

  void initVulkan();
  void mainLoop();
  void cleanup();
  void createInstance();

  void createLogicalDevice();
};

#endif
