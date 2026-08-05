#include "triangleApplication.hpp"

void TriangleApplication::initVulkan() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Scop", nullptr, nullptr);
	
}

void TriangleApplication::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void TriangleApplication::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

TriangleApplication::TriangleApplication() {}

TriangleApplication::~TriangleApplication() {}
