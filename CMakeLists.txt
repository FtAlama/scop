cmake_minimum_required(VERSION 3.22)
project(hyper_scop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(scop 
	src/main.cpp
	src/0_setup/0_triangleApp.cpp
	src/0_setup/1_validationLayers.cpp
	src/0_setup/2_windowSurface.cpp
	src/0_setup/3_physicalDevice.cpp
	src/0_setup/4_logicalDevice.cpp
	src/0_setup/5_swapChain.cpp
	src/0_setup/6_imageViews.cpp
	src/1_pipeline/0_renderPass.cpp
	src/1_pipeline/1_graphicsPipeline.cpp
	src/2_frames/0_framebuffers.cpp
)

target_include_directories(scop SYSTEM PRIVATE $ENV{HOME}/glm-test)
target_include_directories(scop PRIVATE include)
target_include_directories(scop PRIVATE include/setup)
target_include_directories(scop SYSTEM PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(scop PRIVATE Vulkan::Vulkan glfw)
target_compile_options(scop PRIVATE -Wall -Wextra -Werror -g2 -D_GLIBCXX_DEBUG)
