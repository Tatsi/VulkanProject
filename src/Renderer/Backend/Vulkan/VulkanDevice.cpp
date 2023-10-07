#include "VulkanDevice.h"

#include "VulkanSwapchain.h"

#include <set>

namespace
{

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

VkPhysicalDeviceFeatures getNeededPhysicalDeviceFeatures()
{
    VkPhysicalDeviceFeatures neededFeatures{};
    neededFeatures.textureCompressionBC = true;
    neededFeatures.multiDrawIndirect = true;
    return neededFeatures;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool isPhysicalDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    const Vulkan::QueueFamilyIndices familyIndices = Vulkan::findSuitableQueueFamilies(device, surface);

    VkPhysicalDeviceFeatures neededFeatures = getNeededPhysicalDeviceFeatures();

    const bool neededExtensionsAreSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (neededExtensionsAreSupported)
    {
        Vulkan::SwapChainSupportInfo swapChainSupport = Vulkan::querySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    bool deviceIsSuitable = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    deviceIsSuitable &= familyIndices.graphicsAndComputeFamily.has_value() && familyIndices.presentFamily.has_value();
    deviceIsSuitable &= neededExtensionsAreSupported;
    deviceIsSuitable &= swapChainAdequate;

    // These need to match getNeededPhysicalDeviceFeatures()
    if (neededFeatures.textureCompressionBC && !deviceFeatures.textureCompressionBC)
    {
        deviceIsSuitable = false;
    }
    if (neededFeatures.multiDrawIndirect && !deviceFeatures.multiDrawIndirect)
    {
        deviceIsSuitable = false;
    }

    return deviceIsSuitable;
};

} // namespace

namespace Vulkan
{
VkPhysicalDevice selectPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find a GPU with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isPhysicalDeviceSuitable(device, surface))
        {
            return device;
        }
    }
    throw std::runtime_error("failed to find a suitable GPU with Vulkan support!");
}

QueueFamilyIndices findSuitableQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices{};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        const auto& queueFamily = queueFamilies[i];

        const bool supportGraphicsAndCompute = queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT;

        if (supportGraphicsAndCompute)
        {
            indices.graphicsAndComputeFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (supportGraphicsAndCompute && presentSupport)
        {
            return indices;
        }
    }

    if (!indices.graphicsAndComputeFamily.has_value() || !indices.presentFamily.has_value())
    {
        throw std::runtime_error("failed to find suitable queue families!");
    }

    return indices;
}

VkDevice createLogicalDevice(VkPhysicalDevice& physicalDevice, const QueueFamilyIndices& suitableQueueFamilyIndices, const std::vector<const char*>& validationLayers)
{
    const float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::set<uint32_t> uniqueQueueFamilies = {suitableQueueFamilyIndices.graphicsAndComputeFamily.value(), suitableQueueFamilyIndices.presentFamily.value()};

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = getNeededPhysicalDeviceFeatures();

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // In Vulkan it's no longer needed to set validation layers also for logical device,
    // but done here to support older Vulkan implementations
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    VkDevice device = VK_NULL_HANDLE;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }
    return device;
}


} // namespace Vulkan
