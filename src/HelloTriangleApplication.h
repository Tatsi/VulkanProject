#ifndef VULKANPROJECT_HELLOTRIANGLEAPPLICATION_H
#define VULKANPROJECT_HELLOTRIANGLEAPPLICATION_H

#include "CPUResourceManager.h"
#include "Renderer/Renderer.h"
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

    CPUResourceManager m_cpuResourceManager;
    Window m_window;
    Renderer m_renderer;
};

#endif // VULKANPROJECT_HELLOTRIANGLEAPPLICATION_H
