#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>

class ShaderModule
{
  public:
    ShaderModule();
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file that exists within the
    "SHADER_SOURCE_DIRECTORY_PATH". Valid inputs are either an absolute path to
    the file or a path relative to the "SHADER_SOURCE_DIRECTORY_PATH".
    */
    ShaderModule(std::filesystem::path sourceFilePath);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file that exists within the
    "SHADER_SOURCE_DIRECTORY_PATH". Valid inputs are either an absolute path to
    the file or a path relative to the "SHADER_SOURCE_DIRECTORY_PATH".
    */
    ShaderModule(std::string sourceFilePath);

    /*
    Sets the path to the shader source code. The specified path has to point to
    a valid file that exists within the "SHADER_SOURCE_DIRECTORY_PATH". Valid
    inputs are either an absolute path to the file or a path relative to the
    "SHADER_SOURCE_DIRECTORY_PATH".
    */
    void setSourceFilePath(std::filesystem::path sourceFilePath);
    /*
    Sets the path to the shader source code. The specified path has to point to
    a valid file that exists within the "SHADER_SOURCE_DIRECTORY_PATH". Valid
    inputs are either an absolute path to the file or a path relative to the
    "SHADER_SOURCE_DIRECTORY_PATH".
    */
    void setSourceFilePath(std::string sourceFilePath);

    void compile();  // TODO

    void destroy(VkDevice& device);

  private:
    VkShaderModule handle = VK_NULL_HANDLE;

    // stores path of the shader source relative to "SHADER_SOURCE_DIRECTORY_PATH"
    std::filesystem::path sourceFilePath;
};