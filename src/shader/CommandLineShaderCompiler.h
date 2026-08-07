#pragma once

#include "ShaderCompiler.h"

// TODO: add docs to the class
class CommandLineShaderCompiler : public ShaderCompiler
{
  public:
    bool compileShader(ShaderModule& shaderModule);
};