#ifndef WINDOW_SURFACE_HPP

#define WINDOW_SURFACE_HPP

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "vk_context.hpp"

class WindowSurface {
public:
	WindowSurface(vk_context &, GLFWwindow *);
	~WindowSurface();
private:
	vk_context &surface_ctx;
};

#endif
