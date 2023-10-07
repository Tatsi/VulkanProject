//
// Created by janttala on 6.10.2023.
//

#ifndef VULKANPROJECT_RENDERER_H
#define VULKANPROJECT_RENDERER_H

#include "../CPUResourceManager.h"
#include "../Window.h"
#include "Backend/Vulkan/VulkanBackend.h"

class Renderer
{
public:
    Renderer(Window& window, const CPUResourceManager& cpuResourceManager);

    void createRenderPipeline(std::string_view vertexShaderPath, std::string_view fragmentShaderPath);
private:
    Vulkan::VulkanBackend m_graphicsBackend;
};


#endif // VULKANPROJECT_RENDERER_H
