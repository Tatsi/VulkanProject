#include "VulkanShader.h"

namespace Vulkan
{

VkShaderModule createShaderModule(VkDevice device, const std::vector<uint32_t>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}

VkShaderModule destroyShaderModule(VkDevice device, VkShaderModule shaderModule)
{
    vkDestroyShaderModule(device, shaderModule, nullptr);
}

}