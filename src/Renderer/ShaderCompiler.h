//
// Created by janttala on 6.10.2023.
//

#ifndef VULKANPROJECT_SHADERCOMPILER_H
#define VULKANPROJECT_SHADERCOMPILER_H

#include <glslang/Public/ShaderLang.h>

#include <cstdint>
#include <string>
#include <vector>

namespace ShaderCompiler
{

struct Shader
{
    std::string name;
    EShLanguage stage;
    std::vector<uint32_t> spirvCode;
};

Shader compileShader(std::string name, std::string path, EShLanguage stage);

}

#endif // VULKANPROJECT_SHADERCOMPILER_H
