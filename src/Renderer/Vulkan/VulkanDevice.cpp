#include "VulkanDevice.h"


bool isPhysicalDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    const std::optional<uint32_t> suitableFamilyIndex = findSuitableQueueFamily(device);

    bool deviceIsSuitable = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && suitableFamilyIndex.has_value();

    VkPhysicalDeviceFeatures neededFeatures = getNeededPhysicalDeviceFeatures();

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

VkPhysicalDevice selectPhysicalDevice(VkInstance& instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find a GPU with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isPhysicalDeviceSuitable(device))
        {
            return device;
        }
    }
    throw std::runtime_error("failed to find a suitable GPU with Vulkan support!");
}

std::optional<uint32_t> findSuitableQueueFamily(VkPhysicalDevice device)
{
    std::optional<uint32_t> suitableFamilyIndex;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        const auto& queueFamily = queueFamilies[i];
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            suitableFamilyIndex = i;
            return suitableFamilyIndex;
        }
    }
    return suitableFamilyIndex;
}

VkDevice createLogicalDevice(VkPhysicalDevice& physicalDevice, uint32_t suitableQueueFamilyIndex, const std::vector<const char*>& validationLayers)
{
    const float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = suitableQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = getNeededPhysicalDeviceFeatures();

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

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

VkPhysicalDeviceFeatures getNeededPhysicalDeviceFeatures()
{
    VkPhysicalDeviceFeatures neededFeatures{};
    neededFeatures.textureCompressionBC = true;
    neededFeatures.multiDrawIndirect = true;
    return neededFeatures;
}