#pragma once

#include <string>
#include <vulkan/vulkan.h>

struct ShaderEntryPoint
{
    std::string           name;
    VkShaderStageFlagBits shaderStage;
};