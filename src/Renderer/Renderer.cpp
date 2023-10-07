#include "Renderer.h"

#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

#include "ShaderCompiler.h"


Renderer::Renderer(Window& window, const CPUResourceManager& cpuResourceManager) :
    m_graphicsBackend(debug,
                      window.getResolution(),
                      std::bind(&Window::createVulkanSurface, &window, std::placeholders::_1),
                      window.getRequiredVulkanExtensions(debug))
{
};

void Renderer::createRenderPipeline(std::string_view vertexShaderPath, std::string_view fragmentShaderPath)
{
    ShaderCompiler shaderCompiler;

    const Shader vertexShader = shaderCompiler.compileShader("vertexShader", "shaders/shader.vert", EShLanguage::EShLangVertex);
    const Shader fragmentShader = shaderCompiler.compileShader("fragmentShader", "shaders/shader.frag", EShLanguage::EShLangFragment);

    m_graphicsBackend.createGraphicsPipeline(vertexShader.spirvCode, fragmentShader.spirvCode);
    m_graphicsBackend.createFramebuffers();
}