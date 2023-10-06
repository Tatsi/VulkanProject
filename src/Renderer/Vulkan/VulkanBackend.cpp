//
// Created by janttala on 6.10.2023.
//

#include "VulkanBackend.h"

#include "VulkanDebug.h"
#include "VulkanDevice.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>


namespace Vulkan
{

VulkanBackend::VulkanBackend(bool enableDebug, std::function<VkSurfaceKHR(VkInstance&)> surfaceCreationFunction, std::vector<const char*> windowVulkanExtensions) :
    m_enableDebug(enableDebug)
{
    createInstance(windowVulkanExtensions);

    if (m_enableDebug)
    {
        setupDebugMessenger(m_instance, m_debugMessenger);
    }

    m_surface = surfaceCreationFunction(m_instance);
    m_physicalDevice = selectPhysicalDevice(m_instance, m_surface);
    const QueueFamilyIndices queueFamilies = findSuitableQueueFamilies(m_physicalDevice, m_surface);
    m_device = createLogicalDevice(m_physicalDevice, queueFamilies, getValidationLayers());

    vkGetDeviceQueue(m_device, queueFamilies.graphicsAndComputeFamily.value(), 0, &m_queueGraphicsCompute);
    vkGetDeviceQueue(m_device, queueFamilies.presentFamily.value(), 0, &m_queuePresent);
}

VulkanBackend::~VulkanBackend()
{
    if (m_debugMessenger)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_instance, m_debugMessenger, nullptr);
        }
    }
    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanBackend::createInstance(const std::vector<const char*>& neededInstanceExtensions)
{
    const std::vector<const char*> validationLayers = getValidationLayers();

    // Check that defined validation layers are available if in use
    if (m_enableDebug && !checkValidationLayerSupport(validationLayers))
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan project";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Jongine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

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

    if (m_enableDebug)
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

    if (vkCreateInstance(&createInfo, nullptr, &m_instance))
    {
        throw std::runtime_error("Failed to create a Vulkan physical device!");
    }
}



}