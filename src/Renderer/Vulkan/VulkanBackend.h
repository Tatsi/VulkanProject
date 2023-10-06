#ifndef VULKANPROJECT_VULKANBACKEND_H
#define VULKANPROJECT_VULKANBACKEND_H

#include "VulkanSwapchain.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <functional>
#include <vector>

namespace Vulkan
{
class VulkanBackend
{
public:
    VulkanBackend(bool enableDebug, glm::uvec2 resolution, std::function<VkSurfaceKHR(VkInstance&)> surfaceCreationFunction, std::vector<const char*> windowVulkanExtensions);
    ~VulkanBackend();

    void createGraphicsPipeline(const std::vector<uint32_t>& vertexShaderSpirV, const std::vector<uint32_t>& fragmentShaderSpirV);

private:
    void createInstance(const std::vector<const char*>& neededInstanceExtensions);

    bool m_enableDebug{false};

    SwapChainInfo m_swapchainInfo{};
    std::vector<VkImageView> m_swapchainImageViews;
    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VkQueue m_queueGraphicsCompute{VK_NULL_HANDLE};
    VkQueue m_queuePresent{VK_NULL_HANDLE};
    VkRenderPass m_renderPass{VK_NULL_HANDLE};
    VkPipelineLayout m_pipelineLayout{VK_NULL_HANDLE};
    VkPipeline m_pipeline;
    VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};
};
} // namespace Vulkan


#endif // VULKANPROJECT_VULKANBACKEND_H
