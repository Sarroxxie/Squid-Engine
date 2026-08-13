#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>

struct ShaderEntryPoint
{
    std::string           name;
    VkShaderStageFlagBits shaderStage;
};


// TODO: add docs to the class
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

    // stores path of the shader source relative to "SHADER_SOURCE_DIRECTORY_PATH"
    std::filesystem::path sourceFilePath;

    // TODO: should this be a map instead to specify the kinds of entry points?
    std::vector<ShaderEntryPoint> entryPoints;

  private:
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
};