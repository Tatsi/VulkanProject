#include "Renderer.h"

Renderer::Renderer(const Window& window, const CPUResourceManager& cpuResourceManager) :
    m_graphicsBackend(window.getRequiredVulkanExtensions())
{

};