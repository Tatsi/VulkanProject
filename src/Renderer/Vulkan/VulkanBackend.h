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
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger = nullptr;
};
} // namespace Vulkan


#endif // VULKANPROJECT_VULKANBACKEND_H
