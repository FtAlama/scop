#ifndef VALIDATION_LAYVERS_HPP

#define VALIDATION_LAYVERS_HPP

#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>

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

class ValidationLayers {
public:
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &);
  bool checkValidationSupport();
  std::vector<const char *> getRequiredExtensions();
  void setupDebugMessenger(VkInstance &);
  VkDebugUtilsMessengerEXT &getDebugMsg();
	~ValidationLayers();

private:
	VkInstance instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
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
};

#endif
