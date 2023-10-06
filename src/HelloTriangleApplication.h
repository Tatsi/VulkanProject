#ifndef VULKANPROJECT_HELLOTRIANGLEAPPLICATION_H
#define VULKANPROJECT_HELLOTRIANGLEAPPLICATION_H

#include "VulkanBackend.h"
#include "Window.h"

#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>


class HelloTriangleApplication
{
public:
    HelloTriangleApplication();

    void run();

private:
    void mainLoop();

    Window m_window;
    VulkanBackend m_graphicsBackend;
};

#endif // VULKANPROJECT_HELLOTRIANGLEAPPLICATION_H
