#ifndef VULKANPROJECT_VULKANIMAGE_H
#define VULKANPROJECT_VULKANIMAGE_H

#include <vulkan/vulkan.hpp>

#include <vector>

namespace Vulkan
{

std::vector<VkImageView> createImageViewsForImages(VkDevice logicalDevice, const std::vector<VkImage>& images, VkFormat format);

}


#endif // VULKANPROJECT_VULKANIMAGE_H
