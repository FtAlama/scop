#include "setup/0_triangleApplication.hpp"
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
  window.emplace(WIDTH, HEIGHT, "Scop");
  if (!window)
    throw std::runtime_error("failed to create GLFW window");

  instance.emplace(ctx, layers);
  layers.setupDebugMessenger(ctx.instance);
  win_surface.emplace(ctx, window->get());
  vk_Pdevice.emplace(ctx);
  vk_Ldevice.emplace(ctx);
  swapchain.emplace(ctx, window->get());
  imageView.emplace(ctx, swapchain->getChainImage(),
                    swapchain->getSwapChainImageFormat());
  renderPass.emplace(ctx);
  graphicsPipeline.emplace(ctx);
  framebuffers.emplace(ctx);
  commandPool.emplace(ctx);
  commandBuffer.emplace(ctx);
  syncObjects.emplace(ctx);
}

void TriangleApplication::mainLoop() {
  while (!window->shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
	vkDeviceWaitIdle(ctx.device);
}

void TriangleApplication::drawFrame() {
  vkWaitForFences(ctx.device, 1, &ctx.inFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(ctx.device, 1, &ctx.inFlightFence);

  uint32_t imageIndex;
  vkAcquireNextImageKHR(ctx.device, ctx.swapChain, UINT64_MAX,
                        ctx.imageAvailableSemaphore, VK_NULL_HANDLE,
                        &imageIndex);

	vkResetCommandBuffer(ctx.commandBuffer, 0);
	commandBuffer->recordCommandBuffer(imageIndex);
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {ctx.imageAvailableSemaphore};

	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &ctx.commandBuffer;

	VkSemaphore signalSemaphores[] = {ctx.renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(ctx.graphicsQueue, 1, &submitInfo, ctx.inFlightFence) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapchains[] = {ctx.swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	vkQueuePresentKHR(ctx.presentQueue, &presentInfo);
}

Instance::Instance(vk_context &ctx, ValidationLayers &layers) : ctx(ctx) {
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

  if (vkCreateInstance(&createInfo, nullptr, &ctx.instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

Instance::~Instance() { vkDestroyInstance(ctx.instance, nullptr); }
