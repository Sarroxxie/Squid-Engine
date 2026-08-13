#include "ShaderModule.h"

#include "output/Logger.h"
#include "output/VulkanCheck.h"
#include "Exceptions.h"

ShaderModule::ShaderModule(const std::filesystem::path&   sourceFilePath,
                           const std::vector<ShaderEntryPoint> entryPoints) {
    // choose either backslashes (on Windows) or forward slashes (on POSIX)
    std::filesystem::path path = std::filesystem::path(sourceFilePath).make_preferred();

    check<ShaderCreationException>(path.is_relative(),
                                   "Shader source file path \"" + path.string()
                                       + "\" is not a relative path." + " Only paths relative to the shader source directory (\""
                                       + SHADER_SOURCE_DIRECTORY_PATH + "\") are supported.");

    // check if the shader file exists
    check<ShaderCreationException>(
        std::filesystem::exists(std::filesystem::path(SHADER_SOURCE_DIRECTORY_PATH) / path),
        "Shader source file \"" + path.string() + "\" does not exist in the shader source directory (\""
            + SHADER_SOURCE_DIRECTORY_PATH + "\").");

    this->sourceFilePath = path;
    this->entryPoints    = std::vector<ShaderEntryPoint>(entryPoints);
}

ShaderModule::ShaderModule(const std::string&             sourceFilePath,
                           const std::vector<ShaderEntryPoint> entryPoints)
    : ShaderModule(std::filesystem::path(sourceFilePath), entryPoints) {}

/*
void ShaderModule::destroy(VkDevice& device) {
    if(handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, handle, nullptr);
        SLOG_INFO("Destroyed Shader " << sourceFilePath.string());
    }
}
*/