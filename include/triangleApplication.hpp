#ifndef TRIANGLE_APPLICATION_HPP

#define TRIANGLE_APPLICATION_HPP

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

  void initVulkan();
  void mainLoop();
  void cleanup();
};

#endif
