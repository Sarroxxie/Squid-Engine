#include "ShaderModule.h"

#include "output/Logger.h"
#include "output/VulkanCheck.h"
#include "Exceptions.h"

ShaderModule::ShaderModule() {}

ShaderModule::ShaderModule(const std::filesystem::path& sourceFilePath) {
    setSourceFilePath(sourceFilePath);
}

ShaderModule::ShaderModule(const std::string& sourceFilePath)
    : ShaderModule(std::filesystem::path(sourceFilePath)) {}

ShaderModule::ShaderModule(const std::filesystem::path&   sourceFilePath,
                           const std::vector<std::string> entryPoints)
    : ShaderModule(sourceFilePath) {
    setEntryPoints(entryPoints);
}

ShaderModule::ShaderModule(const std::string&             sourceFilePath,
                           const std::vector<std::string> entryPoints)
    : ShaderModule(std::filesystem::path(sourceFilePath), entryPoints) {}

VkShaderModule ShaderModule::getHandle() const {
    return handle;
}

std::filesystem::path ShaderModule::getSourceFilePath() const {
    return sourceFilePath;
}

std::vector<std::string> ShaderModule::getEntryPoints() {
    return entryPoints;
}

void ShaderModule::setSourceFilePath(const std::filesystem::path& sourceFilePath) {
    // choose either backslashes (on Windows) or forward slashes (on POSIX)
    std::filesystem::path path = std::filesystem::path(sourceFilePath).make_preferred();

    if(path.is_absolute()) {
        // check if the shader file exists
        check<ShaderCreationException>(std::filesystem::exists(path),
                                       "Shader source file \"" + path.string() + "\" does not exist.");

        // check if the shader file lies within the shader source directory
        std::filesystem::path relativePath =
            std::filesystem::relative(path, SHADER_SOURCE_DIRECTORY_PATH);

        check<ShaderCreationException>(!relativePath.empty(),
                                       "Shader source file \"" + path.string() + "\" does not exist within the shader source directory ("
                                           + SHADER_SOURCE_DIRECTORY_PATH + ").");

        this->sourceFilePath = relativePath;
    } else {
        // check if the shader file exists
        check<ShaderCreationException>(
            std::filesystem::exists(std::filesystem::path(SHADER_SOURCE_DIRECTORY_PATH) / path),
            "Shader source file \"" + path.string() + "\" does not exist in the shader source directory ("
                + SHADER_SOURCE_DIRECTORY_PATH + ").");

        this->sourceFilePath = path;
    }
}

void ShaderModule::setSourceFilePath(const std::string& sourceFilePath) {
    setSourceFilePath(std::filesystem::path(sourceFilePath));
}

void ShaderModule::addEntryPoint(const std::string& entryPoint) {
    entryPoints.push_back(entryPoint);
}

void ShaderModule::addEntryPoints(const std::vector<std::string>& entryPoints) {
    this->entryPoints.reserve(this->entryPoints.size() + entryPoints.size());
    this->entryPoints.insert(std::end(this->entryPoints),
                             std::begin(entryPoints), std::end(entryPoints));
}

void ShaderModule::setEntryPoints(const std::vector<std::string>& entryPoints) {
    this->entryPoints = std::vector<std::string>(entryPoints);
}

void ShaderModule::destroy(VkDevice& device) {
    if(handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, handle, nullptr);
        SLOG_INFO("Destroyed Shader " << sourceFilePath.string());
    }
}