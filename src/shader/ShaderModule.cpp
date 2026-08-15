#include "ShaderModule.h"

#include "output/Logger.h"
#include "output/VulkanCheck.h"
#include "Exceptions.h"
#include <fstream>

ShaderModule::ShaderModule(const std::filesystem::path&        sourceFilePath,
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

ShaderModule::ShaderModule(const std::string&                  sourceFilePath,
                           const std::vector<ShaderEntryPoint> entryPoints)
    : ShaderModule(std::filesystem::path(sourceFilePath), entryPoints) {}

const std::vector<char> ShaderModule::readSpirvShaderFile() {
    std::filesystem::path filePath =
        std::filesystem::path(SHADER_COMPILE_DIRECTORY_PATH) / sourceFilePath;
    filePath.replace_extension("spv");

    std::ifstream file(filePath, std::ios::ate | std::ios::binary);
    check<ShaderModuleCreationException>(file.is_open(), "Opening shader code file failed.");

    size_t            fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void ShaderModule::createVulkanShaderModule(const VkDevice&          device,
                                            const std::vector<char>& shaderCode,
                                            const VkShaderModuleCreateFlags flags) {
    VkShaderModuleCreateInfo createInfo{
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = nullptr,
        .flags    = flags,
        .codeSize = shaderCode.size(),
        .pCode    = reinterpret_cast<const uint32_t*>(shaderCode.data())};

    check<ShaderModuleCreationException>(vkCreateShaderModule(device, &createInfo,
                                                              nullptr, &handle),
                                         "Shader Module creation failed.");
}

bool ShaderModule::compile(const ShaderCompiler* compiler) const {
    return compiler->compile(sourceFilePath, entryPoints);
}

void ShaderModule::buildShaderStages(const VkDevice& device) {
    auto shaderCode = readSpirvShaderFile();
    createVulkanShaderModule(device, shaderCode, 0);

    shaderStages.reserve(entryPoints.size());

    for(ShaderEntryPoint entryPoint : entryPoints) {
        VkPipelineShaderStageCreateInfo shaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = entryPoint.shaderStage,
            .module = handle,
            .pName  = entryPoint.name.c_str(),
            .pSpecializationInfo = nullptr};
        shaderStages.push_back(shaderStageCreateInfo);
    }
}

std::vector<ShaderEntryPoint> ShaderModule::getEntryPoints() const {
    // copy to avoid modifying member variable
    return std::vector<ShaderEntryPoint>(entryPoints);
}

VkShaderModule ShaderModule::getHandle() const {
    // copy to avoid modifying member variable
    return VkShaderModule(handle);
}

std::filesystem::path ShaderModule::getSourceFilePath() const {
    // copy to avoid modifying member variable
    return std::filesystem::path(sourceFilePath);
}

void ShaderModule::destroy(VkDevice& device) {
    if(handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, handle, nullptr);
        SLOG_INFO("Destroyed Shader \"" << sourceFilePath.string() << "\"");
    }
}