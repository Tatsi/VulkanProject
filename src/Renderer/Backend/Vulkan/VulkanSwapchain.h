#ifndef VULKANPROJECT_VULKANSWAPCHAIN_H
#define VULKANPROJECT_VULKANSWAPCHAIN_H

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <array>
#include <vector>

namespace Vulkan
{

struct SwapChainInfo
{
    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    VkExtent2D extent;
    VkSurfaceFormatKHR format;
};

struct SwapChainSupportInfo
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportInfo querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
SwapChainInfo createSwapChain(VkPhysicalDevice physicalDevice,
                              VkDevice logicalDevice,
                              VkSurfaceKHR surface,
                              glm::uvec2 windowResolution,
                              std::array<uint32_t, 2> queueFamilyIndices);

} // namespace Vulkan

#endif // VULKANPROJECT_VULKANSWAPCHAIN_H
