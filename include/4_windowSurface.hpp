#ifndef WINDOW_SURFACE_HPP

#define WINDOW_SURFACE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>


class WindowSurface {
public:
	void createSurface(VkInstance &, GLFWwindow *);
	void destroySurface(VkInstance &);
	VkSurfaceKHR &get_surface();
private:
	VkSurfaceKHR surface;
};

#endif
