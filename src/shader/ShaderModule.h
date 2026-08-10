#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>


// TODO: add docs to the class
class ShaderModule
{
  public:
    ShaderModule();
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file relative to "SHADER_SOURCE_DIRECTORY_PATH".
    */
    ShaderModule(const std::filesystem::path& sourceFilePath);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file relative to "SHADER_SOURCE_DIRECTORY_PATH".
    */
    ShaderModule(const std::string& sourceFilePath);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file relative to "SHADER_SOURCE_DIRECTORY_PATH".
    Also stores the Entry Points (that should belong to the specified shader).
    */
    ShaderModule(const std::filesystem::path&   sourceFilePath,
                 const std::vector<std::string> entryPoints);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file relative to "SHADER_SOURCE_DIRECTORY_PATH".
    Also stores the Entry Points (that should belong to the specified shader).
    */
    ShaderModule(const std::string& sourceFilePath, const std::vector<std::string> entryPoints);

    // stores path of the shader source relative to "SHADER_SOURCE_DIRECTORY_PATH"
    std::filesystem::path sourceFilePath;

    // TODO: should this be a map instead to specify the kinds of entry points?
    std::vector<std::string> entryPoints;

    /*
    Returns the Vulkan Handle to the Shader Module.
    */
    VkShaderModule getHandle() const;

    /*
    Destroys resources created by Vulkan.
    */
    void destroy(VkDevice& device);

  private:
    VkShaderModule handle = VK_NULL_HANDLE;
};