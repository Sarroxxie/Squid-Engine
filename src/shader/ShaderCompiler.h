#pragma once

#include "ShaderEntryPoint.h"
#include <filesystem>

// TODO: add a "compileIfChanged" function (that internally uses "compile")
//       -> reduces unnecessary compute

/*
Manages shader compilation. The compiled shader will be a SPIR-V file inside the
"SHADER_COMPILE_DIRECTORY_PATH" directory.
*/
class ShaderCompiler
{
  public:
    /*
    Compiles the shader source file into a SPIR-V file. The resulting file is
    put into the "SHADER_COMPILE_DIRECTORY_PATH" directory under the same
    relative path as the shader source file. Returns true if the compilation was
    successful, false if an error occurred.
    */
    virtual bool compile(const std::filesystem::path sourceFilePath,
                         const std::vector<ShaderEntryPoint> entryPoints) const = 0;
};

class CommandLineShaderCompiler : public ShaderCompiler
{
  public:
    /*
    Compiles the shader source file into a SPIR-V file using "slangc" via system
    call in the command line. The resulting file is put into the
    "SHADER_COMPILE_DIRECTORY_PATH" directory under the same relative path as
    the shader source file. Returns true if the compilation was successful,
    false if an error occurred.
    */
    bool compile(const std::filesystem::path sourceFilePath,
                 const std::vector<ShaderEntryPoint> entryPoints) const override;
};