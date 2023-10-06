//
// Created by janttala on 6.10.2023.
//

#include "VulkanBackend.h"
#include "Config.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};
}


VulkanBackend::VulkanBackend(std::vector<const char*> windowVulkanExtensions)
{
    createInstance(windowVulkanExtensions);
    setupDebugMessenger();
}

VulkanBackend::~VulkanBackend()
{
    if (enableValidationLayers)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_instance, m_debugMessenger, nullptr);
        }
    }
}

void VulkanBackend::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void VulkanBackend::setupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    // look address of the extension function and call it
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(m_instance, &createInfo, nullptr, &m_debugMessenger);
    }
    else
    {
        throw std::runtime_error("Failed to setup debug messenger!");
    }
}

void VulkanBackend::createInstance(const std::vector<const char*>& neededInstanceExtensions)
{
    // Check that defined validation layers are available if in use
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan project";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Get supported instance level extensions
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());
    std::cout << "Available extensions:" << std::endl;

    for (const auto& extension : availableExtensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    // Check that all extensions required by our apps are supported
    for (const char* extension : neededInstanceExtensions)
    {
        const std::string neededExtensionName = std::string(extension);
        if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [neededExtensionName](VkExtensionProperties p)
                         { return std::string(p.extensionName) == neededExtensionName; }) == availableExtensions.end())
        {
            throw std::runtime_error("Required extension " + neededExtensionName + " is not supported!");
        }
    }

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(neededInstanceExtensions.size());
    createInfo.ppEnabledExtensionNames = neededInstanceExtensions.data();
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        // Enable validation for vkCreateInstance and as vkDestroyInstance the main debug messenger has
        // to be created after the call to vkCreateInstance and before the call to vkDestroyInstance
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanBackend::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }
    return VK_FALSE;
}

bool VulkanBackend::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound)
        {
            return false;
        }
    }
    return true;
}