#pragma once

#include "ShaderModule.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace ShaderStageBuilder {
std::vector<VkPipelineShaderStageCreateInfo> buildShaderStages(const VkDevice& device,
                                                               const ShaderModule& shaderModule);
};