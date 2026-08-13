#pragma once

#include "ShaderModule.h"

/*
Contains different variants of shader compilation.
*/
namespace ShaderCompiler {

      namespace CommandLine{
            /*
            Compile shader using the command line.
            */
            bool compile(const ShaderModule& shaderModule);
      }
}