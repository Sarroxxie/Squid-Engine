#include "VulkanCheck.h"

#include <vulkan/vulkan.h>
#include <stdexcept>

void check(VkResult result) {
    if(result != VK_SUCCESS)
        throw std::runtime_error("Vulkan call returned an error.\n");
}

void check(VkResult result, std::string message) {
    if(result != VK_SUCCESS)
        throw std::runtime_error(message);
}

void check(bool result) {
    if(!result)
        throw std::runtime_error("Call returned an error.\n");
}

void check(bool result, std::string message) {
    if(!result)
        throw std::runtime_error(message);
}