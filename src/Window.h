#ifndef VULKANPROJECT_WINDOW_H
#define VULKANPROJECT_WINDOW_H


#include <vector>

struct GLFWwindow;

class Window
{
public:
    Window(size_t width, size_t height);
    ~Window();

    bool update();

    std::vector<const char*> getRequiredVulkanExtensions() const;

private:
    GLFWwindow* m_window;
};


#endif // VULKANPROJECT_WINDOW_H
