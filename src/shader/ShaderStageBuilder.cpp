#include "ShaderStageBuilder.h"

#include "output/VulkanCheck.h"
#include "Exceptions.h"
#include <fstream>

#include <vector>
#include <vulkan/vulkan.h>

#include <iostream>

const std::vector<char> readSpirvShaderFile(const ShaderModule& shaderModule) {
    std::filesystem::path filePath =
        std::filesystem::path(SHADER_COMPILE_DIRECTORY_PATH) / shaderModule.sourceFilePath;
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

VkShaderModule createVulkanShaderModule(const VkDevice&          device,
                                        const std::vector<char>& shaderCode,
                                        const VkShaderModuleCreateFlags flags) {
    VkShaderModuleCreateInfo createInfo{
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = nullptr,
        .flags    = flags,
        .codeSize = shaderCode.size(),
        .pCode    = reinterpret_cast<const uint32_t*>(shaderCode.data())};

    VkShaderModule shaderModule;
    check<ShaderModuleCreationException>(vkCreateShaderModule(device, &createInfo,
                                                              nullptr, &shaderModule),
                                         "Shader Module creation failed.");

    return shaderModule;
}

std::vector<VkPipelineShaderStageCreateInfo> ShaderStageBuilder::buildShaderStages(
    const VkDevice&     device,
    const ShaderModule& shaderModule) {
    auto shaderCode = readSpirvShaderFile(shaderModule);
    VkShaderModule vulkanShaderModule = createVulkanShaderModule(device, shaderCode, 0);

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
    shaderStageCreateInfos.reserve(shaderModule.entryPoints.size());

    for(ShaderEntryPoint entryPoint : shaderModule.entryPoints) {
        VkPipelineShaderStageCreateInfo shaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = entryPoint.shaderStage,
            .module = vulkanShaderModule,
            .pName  = entryPoint.name.c_str(),
            .pSpecializationInfo = nullptr};
        shaderStageCreateInfos.push_back(shaderStageCreateInfo);
    }

    vkDestroyShaderModule(device, vulkanShaderModule, nullptr);

    return shaderStageCreateInfos;
}