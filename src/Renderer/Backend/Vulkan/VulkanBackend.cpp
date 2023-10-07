#include "VulkanBackend.h"

#include "VulkanDebug.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanPipeline.h"
#include "VulkanShader.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace Vulkan
{

VulkanBackend::VulkanBackend(bool enableDebug, glm::uvec2 resolution, std::function<VkSurfaceKHR(VkInstance&)> surfaceCreationFunction, std::vector<const char*> windowVulkanExtensions) :
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

    const std::array<uint32_t, 2> queueFamilyIndices = {queueFamilies.graphicsAndComputeFamily.value(), queueFamilies.presentFamily.value()};
    m_swapchainInfo = createSwapChain(m_physicalDevice, m_device, m_surface, resolution, queueFamilyIndices);

    m_swapchainImageViews = createImageViewsForImages(m_device, m_swapchainInfo.images, m_swapchainInfo.format.format);
}

VulkanBackend::~VulkanBackend()
{
    vkDestroyPipeline(m_device, m_pipeline, nullptr);
    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);

    const std::vector<VkFramebuffer*> aliveFramebuffers = m_framebuffers.getAliveData();
    for (const VkFramebuffer* framebuffer : aliveFramebuffers)
    {
        vkDestroyFramebuffer(m_device, *framebuffer, nullptr);
    }
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    for (auto swapchainImageView : m_swapchainImageViews)
    {
        vkDestroyImageView(m_device, swapchainImageView, nullptr);
    }

    vkDestroySwapchainKHR(m_device, m_swapchainInfo.swapchain, nullptr);
    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    if (m_debugMessenger)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_instance, m_debugMessenger, nullptr);
        }
    }
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

void VulkanBackend::createGraphicsPipeline(const std::vector<uint32_t>& vertexShaderSpirV, const std::vector<uint32_t>& fragmentShaderSpirV)
{
    VkShaderModule vertexShaderModule = createShaderModule(m_device, vertexShaderSpirV);
    VkShaderModule fragmentShaderModule = createShaderModule(m_device, fragmentShaderSpirV);

    if (m_renderPass)
    {
        throw std::runtime_error("Currently only one render pass is supported!");
    }
    m_renderPass = createRenderPass(m_device, m_swapchainInfo.format.format);

    if (m_pipelineLayout)
    {
        throw std::runtime_error("Currently only one pipeline is supported!");
    }
    m_pipelineLayout = createPipelineLayout(m_device);

    m_pipeline = createVulkanGraphicsPipeline(m_device, m_pipelineLayout, m_renderPass, vertexShaderModule, fragmentShaderModule, m_swapchainInfo.extent);

    destroyShaderModule(m_device, vertexShaderModule);
    destroyShaderModule(m_device, fragmentShaderModule);
}

std::vector<Handle<HandleType::Framebuffer>> VulkanBackend::createFramebuffers()
{
    std::vector<Handle<HandleType::Framebuffer>> returnedHandles;
    returnedHandles.reserve(m_swapchainImageViews.size());

    for (size_t i = 0; i < m_swapchainImageViews.size(); i++) {
        VkImageView attachments[] = {
            m_swapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapchainInfo.extent.width;
        framebufferInfo.height = m_swapchainInfo.extent.height;
        framebufferInfo.layers = 1;

        VkFramebuffer framebuffer;

        if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a framebuffer!");
        }
        Handle<HandleType::Framebuffer> handle = m_framebuffers.insertElement(framebuffer);
        returnedHandles.push_back(handle);
    }
    return returnedHandles;
}

} // namespace Vulkan