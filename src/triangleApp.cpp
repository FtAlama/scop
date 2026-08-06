#include "triangleApplication.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

void TriangleApplication::initVulkan() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(WIDTH, HEIGHT, "Scop", nullptr, nullptr);

  createInstance();
}

void TriangleApplication::mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

void TriangleApplication::createInstance() {
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo createInfo{};
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to creat instance!");
  }

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);

  std::cout << "available extensions\n";
  for (const auto &extension : extensions) {
    std::cout << '\t' << extension.extensionName << std::endl;
  }
}

void TriangleApplication::cleanup() {
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}

TriangleApplication::TriangleApplication() {}

TriangleApplication::~TriangleApplication() {}
