cmake_minimum_required (VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_LIST_DIR}/CMake"
)

include(${CMAKE_CURRENT_LIST_DIR}/CMake/vcpkg.cmake)

project(VulkanProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glfw")

set(TARGET_NAME VulkanTutorial)
add_executable(VulkanTutorial
        src/main.cpp
        src/HelloTriangleApplication.cpp
        src/HelloTriangleApplication.h
		src/Window.cpp
		src/Window.h
		src/Config.h
		src/CPUResourceManager.cpp
		src/CPUResourceManager.h
		src/Renderer/Renderer.cpp
		src/Renderer/Renderer.h
		src/Renderer/ShaderCompiler.cpp
		src/Renderer/ShaderCompiler.h
		src/Renderer/Backend/Vulkan/VulkanBackend.cpp
		src/Renderer/Backend/Vulkan/VulkanBackend.h
		src/Renderer/Backend/Vulkan/VulkanDebug.cpp
		src/Renderer/Backend/Vulkan/VulkanDebug.h
		src/Renderer/Backend/Vulkan/VulkanDevice.cpp
		src/Renderer/Backend/Vulkan/VulkanDevice.h
		src/Renderer/Backend/Vulkan/VulkanSwapchain.cpp
		src/Renderer/Backend/Vulkan/VulkanSwapchain.h
		src/Renderer/Backend/Vulkan/VulkanImage.cpp
		src/Renderer/Backend/Vulkan/VulkanImage.h
		src/Renderer/Backend/Vulkan/VulkanPipeline.cpp
		src/Renderer/Backend/Vulkan/VulkanPipeline.h
		src/Utilities/Filesystem.cpp
		src/Utilities/Filesystem.h
		src/Renderer/Backend/Vulkan/VulkanShader.cpp
		src/Renderer/Backend/Vulkan/VulkanShader.h
		src/Renderer/Backend/Types.h
		src/Renderer/Backend/Handle.cpp
		src/Renderer/Backend/Handle.h
)

find_package(Vulkan REQUIRED)

target_include_directories(VulkanTutorial
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/glm"
        "${CMAKE_CURRENT_LIST_DIR}/glfw/include"
        "${Vulkan_INCLUDE_DIRS}"
)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(Threads REQUIRED)
find_package(glslang REQUIRED)

target_link_libraries(VulkanTutorial PRIVATE
	Vulkan::Vulkan
	glslang::glslang
	glslang::SPIRV
	glfw
	glm::glm
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${CMAKE_CURRENT_LIST_DIR}/shaders $<TARGET_FILE_DIR:${TARGET_NAME}>/shaders)
