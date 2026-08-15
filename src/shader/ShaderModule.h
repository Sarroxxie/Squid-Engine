#pragma once

#include "ShaderEntryPoint.h"
#include "ShaderCompiler.h"
#include <vulkan/vulkan.h>

/*
Wrapper for managing shader compilation, shader module and shader stages creation.
*/
class ShaderModule
{
  public:
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file relative to "SHADER_SOURCE_DIRECTORY_PATH".
    Also stores the Entry Points (that should belong to the specified shader).
    */
    ShaderModule(const std::filesystem::path&        sourceFilePath,
                 const std::vector<ShaderEntryPoint> entryPoints);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file relative to "SHADER_SOURCE_DIRECTORY_PATH".
    Also stores the Entry Points (that should belong to the specified shader).
    */
    ShaderModule(const std::string&                  sourceFilePath,
                 const std::vector<ShaderEntryPoint> entryPoints);

    /*
    Compiles the shader source file into a SPIR-V file by using the specified
    ShaderCompiler. The path to the compiled file is the same as the shader
    source file but relative to "SHADER_COMPILE_DIRECTORY_PATH". Also note that
    the file extension of the compiled shader is ".spv" after compilation.
    Returns true if the compilation was successful, false if an error occurred.
    */
    bool compile(const ShaderCompiler* compiler) const;

    /*
    Builds the shader stages to be used in the graphics pipeline. This function
    should only be called after the shader source file has been compiled into a
    SPIR-V file that has the same path as the shader source file but relative to
    "SHADER_COMPILE_DIRECTORY_PATH".
    */
    void buildShaderStages(const VkDevice& device);

    /*
    Returns all entry points of the ShaderModule.
    */
    std::vector<ShaderEntryPoint> getEntryPoints() const;

    /*
    Returns the Vulkan handle to the ShaderModule.
    */
    VkShaderModule getHandle() const;

    /*
    Returns the path to the shader source code. Note that the returned path is
    relative to "SHADER_SOURCE_DIRECTORY_PATH".
    */
    std::filesystem::path getSourceFilePath() const;

    /*
    Destroys the ShaderModule by destroying the VkShaderModule.
    */
    void destroy(VkDevice& device);

  private:
    // stores path of the shader source relative to "SHADER_SOURCE_DIRECTORY_PATH"
    std::filesystem::path                        sourceFilePath;
    std::vector<ShaderEntryPoint>                entryPoints;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkShaderModule                               handle = VK_NULL_HANDLE;

    const std::vector<char> readSpirvShaderFile();

    void createVulkanShaderModule(const VkDevice&                 device,
                                  const std::vector<char>&        shaderCode,
                                  const VkShaderModuleCreateFlags flags);
};