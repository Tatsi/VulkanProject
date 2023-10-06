//
// Created by janttala on 6.10.2023.
//

#ifndef VULKANPROJECT_VULKANBACKEND_H
#define VULKANPROJECT_VULKANBACKEND_H

#include "vulkan/vulkan.hpp"

class VulkanBackend
{
public:
    VulkanBackend(std::vector<const char*> windowVulkanExtensions);
    ~VulkanBackend();

private:
    void createInstance(const std::vector<const char*>& neededInstanceExtensions);

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    bool checkValidationLayerSupport();

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
};


#endif // VULKANPROJECT_VULKANBACKEND_H
