#include "4_window_surface.hpp"
#include <stdexcept>
#include <iostream>
#include <vulkan/vulkan_core.h>

void WindowSurface::createSurface(VkInstance &instance, GLFWwindow *window) {
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
      VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
	std::cout << "Window surface create\n";
}

void WindowSurface::destroySurface(VkInstance &instance) {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}

VkSurfaceKHR &WindowSurface::get_surface() {
	return (surface);
}
