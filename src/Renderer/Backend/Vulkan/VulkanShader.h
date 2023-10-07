#ifndef VULKANPROJECT_VULKANSHADER_H
#define VULKANPROJECT_VULKANSHADER_H

#include <vulkan/vulkan.hpp>

#include <vector>

namespace Vulkan
{

VkShaderModule createShaderModule(VkDevice device, const std::vector<uint32_t>& code);
VkShaderModule destroyShaderModule(VkDevice device, VkShaderModule shaderModule);

}

#endif // VULKANPROJECT_VULKANSHADER_H
