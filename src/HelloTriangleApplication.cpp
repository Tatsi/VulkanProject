
#include "HelloTriangleApplication.h"


HelloTriangleApplication::HelloTriangleApplication() :
    m_window(800, 600),
    m_graphicsBackend(m_window.getRequiredVulkanExtensions())
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