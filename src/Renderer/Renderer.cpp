#include "Renderer.h"

#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

Renderer::Renderer(const Window& window, const CPUResourceManager& cpuResourceManager) :
    m_graphicsBackend(debug, window.getRequiredVulkanExtensions(debug))
{

};