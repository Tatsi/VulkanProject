#ifndef VULKANPROJECT_VULKANDEVICE_H
#define VULKANPROJECT_VULKANDEVICE_H

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;
};

VkPhysicalDevice selectPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
bool isPhysicalDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices findSuitableQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
VkDevice createLogicalDevice(VkPhysicalDevice& physicalDevice, const QueueFamilyIndices& suitableQueueFamilyIndices, const std::vector<const char*>& validationLayers);
VkPhysicalDeviceFeatures getNeededPhysicalDeviceFeatures();


#endif // VULKANPROJECT_VULKANDEVICE_H
