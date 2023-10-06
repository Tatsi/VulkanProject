#include "Renderer.h"

#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

#include "ShaderCompiler.h"

#include <vulkan/vulkan.h>

Renderer::Renderer(Window& window, const CPUResourceManager& cpuResourceManager) :
    m_graphicsBackend(debug,
                      window.getResolution(),
                      std::bind(&Window::createVulkanSurface, &window, std::placeholders::_1),
                      window.getRequiredVulkanExtensions(debug))
{
    ShaderCompiler::Shader vertexShader = ShaderCompiler::compileShader("vertexShader", "shaders/shader.vert", EShLanguage::EShLangVertex);
    ShaderCompiler::Shader fragmentShader = ShaderCompiler::compileShader("fragmentShader", "shaders/shader.frag", EShLanguage::EShLangFragment);

    //m_graphicsBackend.createGraphicsPipeline(Shader vertexShader, Shader fragmentShader);

};