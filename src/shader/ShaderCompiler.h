#pragma once

#include "ShaderModule.h"
#include <vector>

// TODO: should add a "ShaderManager" if we want to auto compile (that uses this class)
// TODO: add docs to the class
class ShaderCompiler
{
  public:
    virtual bool compileShader(ShaderModule& shaderModule) = 0;
};