#ifndef VULKANPROJECT_VULKANPIPELINE_H
#define VULKANPROJECT_VULKANPIPELINE_H

#include <vulkan/vulkan.hpp>

namespace Vulkan
{

VkPipelineLayout createPipelineLayout(VkDevice device);
VkRenderPass createRenderPass(VkDevice device, VkFormat colorAttachmentFormat);
VkPipeline createVulkanGraphicsPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, VkShaderModule vertexShaderModule, VkShaderModule fragmentShaderModule, VkExtent2D viewportAndScissorExtent);

}


#endif // VULKANPROJECT_VULKANPIPELINE_H
