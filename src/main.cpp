#define GLFW_INCLUDE_VULKAN

#include "HelloTriangleApplication.h"

#include <cstdlib>
#include <iostream>


int main()
{
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}