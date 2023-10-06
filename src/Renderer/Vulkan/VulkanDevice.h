#ifndef VULKANPROJECT_VULKANDEVICE_H
#define VULKANPROJECT_VULKANDEVICE_H

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>


VkPhysicalDevice selectPhysicalDevice(VkInstance& instance);
bool isPhysicalDeviceSuitable(VkPhysicalDevice device);
std::optional<uint32_t> findSuitableQueueFamily(VkPhysicalDevice device);
VkDevice createLogicalDevice(VkPhysicalDevice& physicalDevice, uint32_t suitableQueueFamilyIndex, const std::vector<const char*>& validationLayers);
VkPhysicalDeviceFeatures getNeededPhysicalDeviceFeatures();


#endif // VULKANPROJECT_VULKANDEVICE_H
