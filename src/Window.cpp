#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <stdexcept>

Window::Window(size_t width, size_t height)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(width, height, "Vulkan project", nullptr, nullptr);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::update()
{
    if (glfwWindowShouldClose(m_window))
    {
        return false;
    }
    glfwPollEvents();
    return true;
}

std::vector<const char*> Window::getRequiredVulkanExtensions(bool enableVulkanValidationLayers) const
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableVulkanValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VkSurfaceKHR Window::createVulkanSurface(VkInstance& instance)
{
    VkSurfaceKHR surface{VK_NULL_HANDLE};

    if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    return surface;
}

glm::uvec2 Window::getResolution() const
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    return {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};
}