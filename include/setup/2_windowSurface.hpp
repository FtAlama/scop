#ifndef WINDOW_SURFACE_HPP

#define WINDOW_SURFACE_HPP

#define GLFW_INCLUDE_VULKAN

#include "vk_context.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class WindowSurface {
public:
  WindowSurface(vk_context &, GLFWwindow *);
  ~WindowSurface();

private:
  vk_context &ctx;
};

#endif
