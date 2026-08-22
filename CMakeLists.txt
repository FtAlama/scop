cmake_minimum_required(VERSION 3.22)
project(hyper_scop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(scop 
	src/main.cpp
	src/0_triangleApp.cpp
	src/1_validationLayers.cpp
	src/2_physicalDevice.cpp
	src/3_logicalDevice.cpp
	src/4_windowSurface.cpp
	src/5_swapChain.cpp
	src/6_imageViews.cpp
)

target_include_directories(scop SYSTEM PRIVATE $ENV{HOME}/glm-test)
target_include_directories(scop PRIVATE include)
target_include_directories(scop SYSTEM PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(scop PRIVATE Vulkan::Vulkan glfw)
target_compile_options(scop PRIVATE -Wall -Wextra -Werror)
