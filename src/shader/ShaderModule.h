#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>

// TODO: should we really have private variables and getter/setter or rather
// just make them public?

// TODO: should remove compatibility with absolute paths to simplify (and also
// if absolute paths have to be in the "SHADER_SOURCE_DIRECTORY_PATH" anyways,
// then what is the point of them?)

// TODO: add docs to the class
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
    ShaderModule(const std::filesystem::path& sourceFilePath);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file that exists within the
    "SHADER_SOURCE_DIRECTORY_PATH". Valid inputs are either an absolute path to
    the file or a path relative to the "SHADER_SOURCE_DIRECTORY_PATH".
    */
    ShaderModule(const std::string& sourceFilePath);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file that exists within the
    "SHADER_SOURCE_DIRECTORY_PATH". Valid inputs are either an absolute path to
    the file or a path relative to the "SHADER_SOURCE_DIRECTORY_PATH".
    Also stores the Entry Points (that should belong to the specified shader).
    */
    ShaderModule(const std::filesystem::path&   sourceFilePath,
                 const std::vector<std::string> entryPoints);
    /*
    Creates a ShaderModule and sets its path to the shader source code. The
    specified path has to point to a valid file that exists within the
    "SHADER_SOURCE_DIRECTORY_PATH". Valid inputs are either an absolute path to
    the file or a path relative to the "SHADER_SOURCE_DIRECTORY_PATH".
    Also stores the Entry Points (that should belong to the specified shader).
    */
    ShaderModule(const std::string& sourceFilePath, const std::vector<std::string> entryPoints);

    /*
    Returns the Vulkan Handle to the Shader Module.
    */
    VkShaderModule getHandle() const;
    /*
    Returns the path to the source file of the shader. The returned path is
    relative to the "SHADER_SOURCE_DIRECTORY_PATH".
    */
    std::filesystem::path getSourceFilePath() const;

    std::vector<std::string> getEntryPoints();

    /*
    Sets the path to the shader source code. The specified path has to point to
    a valid file that exists within the "SHADER_SOURCE_DIRECTORY_PATH". Valid
    inputs are either an absolute path to the file or a path relative to the
    "SHADER_SOURCE_DIRECTORY_PATH".
    */
    void setSourceFilePath(const std::filesystem::path& sourceFilePath);
    /*
    Sets the path to the shader source code. The specified path has to point to
    a valid file that exists within the "SHADER_SOURCE_DIRECTORY_PATH". Valid
    inputs are either an absolute path to the file or a path relative to the
    "SHADER_SOURCE_DIRECTORY_PATH".
    */
    void setSourceFilePath(const std::string& sourceFilePath);

    /*
    Adds a single Entry Point (that should belong to the specified shader).
    */
    void addEntryPoint(const std::string& entryPoint);
    /*
    Adds Entry Points (that should belong to the specified shader).
    */
    void addEntryPoints(const std::vector<std::string>& entryPoints);
    /*
    Sets the Entry Points (that should belong to the specified shader).
    */
    void setEntryPoints(const std::vector<std::string>& entryPoints);

    /*
    Destroys resources created by Vulkan.
    */
    void destroy(VkDevice& device);

  private:
    VkShaderModule handle = VK_NULL_HANDLE;

    // stores path of the shader source relative to "SHADER_SOURCE_DIRECTORY_PATH"
    std::filesystem::path sourceFilePath;

    // TODO: should this be a map instead to specify the kinds of entry points?
    std::vector<std::string> entryPoints;
};