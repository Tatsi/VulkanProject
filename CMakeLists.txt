cmake_minimum_required (VERSION 3.15)
project(VulkanProject)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glfw")

add_executable(VulkanTutorial src/main.cpp)

find_package(Vulkan REQUIRED)

target_include_directories(VulkanTutorial
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/glm"
        "${CMAKE_CURRENT_LIST_DIR}/glfw/include"
        "${Vulkan_INCLUDE_DIRS}"
)




target_link_libraries(VulkanTutorial PUBLIC glfw Vulkan::Vulkan)
