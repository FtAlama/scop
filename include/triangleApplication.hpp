#ifndef TRIANGLE_APPLICATION_HPP

#define TRIANGLE_APPLICATION_HPP

#include <iostream>
#include <vector>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "queueFamilies.hpp"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *,
    const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);

void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT,
                                   const VkAllocationCallbacks *);

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
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

  void initVulkan();
  void mainLoop();
  void cleanup();
  void createInstance();
  bool checkValidationSupport();
  std::vector<const char *> getRequiredExtensions();
  void setupDebugMessenger();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &);

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageServity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                void *pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    (void)pUserData;
    (void)messageType;
    (void)messageServity;
    return (false);
  }

  void pickPhysicalDevice();
  int rateDeviceSuitable(VkPhysicalDevice);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
};

#endif
