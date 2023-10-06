
#include "HelloTriangleApplication.h"


HelloTriangleApplication::HelloTriangleApplication() :
    m_cpuResourceManager("assets/test.gltf"),
    m_window(800, 600),
    m_renderer(m_window, m_cpuResourceManager)
{
    m_renderer.createRenderPipeline("shaders/shader.vert", "shaders/shader.frag");
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