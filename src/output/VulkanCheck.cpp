#include "VulkanCheck.h"

#include <stdexcept>

void check(const VkResult result) {
    if(result != VK_SUCCESS)
        throw std::runtime_error("Vulkan call returned an error.\n");
}

void check(const VkResult result, const std::string message) {
    if(result != VK_SUCCESS)
        throw std::runtime_error(message);
}

void check(const bool result) {
    if(!result)
        throw std::runtime_error("Call returned an error.\n");
}

void check(const bool result, const std::string message) {
    if(!result)
        throw std::runtime_error(message);
}