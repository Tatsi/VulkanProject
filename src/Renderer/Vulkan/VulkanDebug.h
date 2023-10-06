//
// Created by janttala on 6.10.2023.
//

#ifndef VULKANPROJECT_VULKANDEBUG_H
#define VULKANPROJECT_VULKANDEBUG_H

#include <vulkan/vulkan.h>

#include <vector>

namespace Vulkan
{

const std::vector<const char*> getValidationLayers();

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

void setupDebugMessenger(VkInstance& m_instance, VkDebugUtilsMessengerEXT& m_debugMessenger);

}

#endif // VULKANPROJECT_VULKANDEBUG_H
