#ifndef VULKANPROJECT_TYPES_H
#define VULKANPROJECT_TYPES_H

#include <cstdint>
#include <limits>
#include <queue>
#include <stack>
#include <stdexcept>

enum class HandleType : uint16_t
{
    Pipeline = 0,
    Framebuffer,
    Image,
    ImageView,
    Buffer,
    BufferView,
    Texture
};


#endif // VULKANPROJECT_TYPES_H
