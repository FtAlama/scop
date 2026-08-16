#include "0_triangleApplication.hpp"
#include "1_validationLayers.hpp"
#include "2_physicalDevice.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

void TriangleApplication::initVulkan() {
  glfwSetErrorCallback([](int code, const char *desc) {
    std::cerr << "GLFW error " << code << ": " << desc << std::endl;
  });
  if (!glfwInit())
    throw std::runtime_error("failed to init GLFW");
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(WIDTH, HEIGHT, "Scop", nullptr, nullptr);
  if (!window)
    throw std::runtime_error("failed to create GLFW window");

  createInstance();
  layers.setupDebugMessenger(instance);
  vk_Pdevice.pickPhysicalDevice(instance);
  vk_Ldevice.createLogicalDevice(vk_Pdevice.getPhysicalDevice());
}

void TriangleApplication::mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

void TriangleApplication::createInstance() {
  if (enableValidationLayers && !layers.checkValidationSupport()) {
    throw std::runtime_error("validation layers request, but not available!");
  }
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo createInfo{};

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions = layers.getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    layers.populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

void TriangleApplication::cleanup() {
  if (enableValidationLayers)
    DestroyDebugUtilsMessengerEXT(instance, layers.getDebugMsg(), nullptr);
  vk_Ldevice.destroyDevice();
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}

TriangleApplication::TriangleApplication() {}

TriangleApplication::~TriangleApplication() {}
