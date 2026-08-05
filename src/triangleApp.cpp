#include "triangleApplication.hpp"

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

void TriangleApplication::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

TriangleApplication::TriangleApplication() {}

TriangleApplication::~TriangleApplication() {}

void TriangleApplication::createInstance() {
	VkApplicationInfo appInfo{};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
}
