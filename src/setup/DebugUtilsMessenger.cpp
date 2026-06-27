#include "DebugUtilsMessenger.h"

#include "output/VulkanCheck.h"
#include "output/Logger.h"
#include "Exceptions.h"

// This method handles all the messages that get passed to the CPU from the Validation Layers.
static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT             messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
              void*                                       userData) {

    // TODO: should map message severity to logger macro
    //       -> also print message type
    SLOG_INFO("VL: " << callbackData->pMessage);

    return VK_FALSE;
}

// Attemps to fetch and call the method "vkCreateDebugUtilsMessengerEXT".
VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
                                      const VkAllocationCallbacks* allocator,
                                      VkDebugUtilsMessengerEXT* debugMessenger) {
    auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");
    if(function != nullptr) {
        return function(instance, createInfo, allocator, debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// Attemps to fetch and call the method "vkDestroyDebugUtilsMessengerEXT".
void destroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     debugMessenger,
                                   const VkAllocationCallbacks* allocator) {
    auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if(function != nullptr) {
        function(instance, debugMessenger, allocator);
    }
}

DebugUtilsMessenger::DebugUtilsMessenger(VkInstance& instance) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    DebugUtilsMessenger::populateDebugUtilsMessengerCreateInfo(createInfo);

    check<DebugUtilsMessengerCreationException>(
        createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &vkDebugMessenger),
        "Failed to create Debug Utils Messenger.");
    SLOG_INFO("Successfully created Debug Utils Messenger.");
}

void DebugUtilsMessenger::cleanup(VkInstance& instance) {
    destroyDebugUtilsMessengerEXT(instance, vkDebugMessenger, nullptr);
    SLOG_INFO("Cleaned up Debug Utils Messenger.");
}

void DebugUtilsMessenger::populateDebugUtilsMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                 // TODO: might enable this conditionally
                                 //| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    // TODO: might enable this conditionally
    //| VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData       = nullptr;
}
