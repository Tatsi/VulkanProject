#ifndef VULKANPROJECT_CPURESOURCEMANAGER_H
#define VULKANPROJECT_CPURESOURCEMANAGER_H

#include <string_view>

/**
 * Holds data loaded to CPU memory; textures, meshes etc.
 */
class CPUResourceManager
{
public:
    CPUResourceManager(std::string_view assetFilePath);
private:
};


#endif // VULKANPROJECT_CPURESOURCEMANAGER_H
