//
// Created by janttala on 6.10.2023.
//

#ifndef VULKANPROJECT_RENDERER_H
#define VULKANPROJECT_RENDERER_H

#include "VulkanBackend.h"
#include "Window.h"
#include "CPUResourceManager.h"

class Renderer
{
public:
    Renderer(const Window& window, const CPUResourceManager& cpuResourceManager);
private:
    VulkanBackend m_graphicsBackend;
};


#endif // VULKANPROJECT_RENDERER_H
