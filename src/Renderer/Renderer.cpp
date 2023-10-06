#include "Renderer.h"

#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

#include <vulkan/vulkan.h>

Renderer::Renderer(Window& window, const CPUResourceManager& cpuResourceManager) :
    m_graphicsBackend(debug, std::bind(&Window::createVulkanSurface, &window, std::placeholders::_1), window.getRequiredVulkanExtensions(debug))
{
};