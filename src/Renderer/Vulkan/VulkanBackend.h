#ifndef VULKANPROJECT_VULKANBACKEND_H
#define VULKANPROJECT_VULKANBACKEND_H

#include "vulkan/vulkan.hpp"

namespace Vulkan
{
class VulkanBackend
{
public:
    VulkanBackend(bool enableDebug, std::vector<const char*> windowVulkanExtensions);
    ~VulkanBackend();

private:
    void createInstance(const std::vector<const char*>& neededInstanceExtensions);

    bool m_enableDebug{false};
    VkInstance m_instance{VK_NULL_HANDLE};
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VkQueue m_queue;
    VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};
};
} // namespace Vulkan


#endif // VULKANPROJECT_VULKANBACKEND_H
