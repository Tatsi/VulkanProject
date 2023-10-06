
#include "HelloTriangleApplication.h"


HelloTriangleApplication::HelloTriangleApplication() :
    m_cpuResourceManager("assets/test.gltf"),
    m_window(800, 600),
    m_renderer(m_window, m_cpuResourceManager)
{
}

void HelloTriangleApplication::run()
{
    mainLoop();
}

void HelloTriangleApplication::mainLoop()
{
    while (m_window.update())
    {
    }
}