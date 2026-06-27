#include "InstanceBuilder.h"
#include "output/VulkanCheck.h"
#include "output/Logger.h"
#include "Exceptions.h"
#include <stdexcept>


InstanceBuilder& InstanceBuilder::setAppName(const char* applicationName) {
    if(!applicationName)
        return *this;
    applicationInfo.pApplicationName = applicationName;
    return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(const uint32_t version) {
    applicationInfo.applicationVersion = version;
    return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(const uint32_t major,
                                                const uint32_t minor,
                                                const uint32_t patch) {
    return setAppVersion(VK_MAKE_VERSION(major, minor, patch));
}

InstanceBuilder& InstanceBuilder::setEngineName(const char* engineName) {
    if(!engineName)
        return *this;
    applicationInfo.pEngineName = engineName;
    return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(const uint32_t version) {
    applicationInfo.engineVersion = version;
    return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(const uint32_t major,
                                                   const uint32_t minor,
                                                   const uint32_t patch) {
    return setEngineVersion(VK_MAKE_VERSION(major, minor, patch));
}

InstanceBuilder& InstanceBuilder::setApiVersion(const uint32_t version) {
    applicationInfo.apiVersion = version;
    return *this;
}

InstanceBuilder& InstanceBuilder::setApiVersion(const uint32_t variant,
                                                const uint32_t major,
                                                const uint32_t minor) {
    // according to Vulkan the patch number should always be set to 0
    return setApiVersion(VK_MAKE_API_VERSION(variant, major, minor, 0));
}

InstanceBuilder& InstanceBuilder::requestLayers(const std::vector<const char*>& layers) {
    if(layers.size() == 0)
        return *this;
    assertLayerSupport(layers);
    createInfo.enabledLayerCount   = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();
    return *this;
}

InstanceBuilder& InstanceBuilder::requestExtensions(const std::vector<const char*>& extensions) {
    if(extensions.size() == 0)
        return *this;
    assertExtensionSupport(extensions);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    return *this;
}

InstanceBuilder& InstanceBuilder::attachDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& messengerCreateInfo) {
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&messengerCreateInfo;
    return *this;
}

void InstanceBuilder::build(VkInstance& instance) const {
    check<InstanceCreationException>(vkCreateInstance(&createInfo, nullptr, &instance),
                                     "Failed to create VkInstance.");
    SLOG_INFO("Successfully created Instance.");
}

void InstanceBuilder::assertLayerSupport(const std::vector<const char*> layers) const {
    uint32_t layerCount;
    check<InstanceCreationException>(vkEnumerateInstanceLayerProperties(&layerCount, nullptr),
                                     "Failed to enumerate Instance Layer Properties.");

    std::vector<VkLayerProperties> availableLayers(layerCount);
    check<InstanceCreationException>(
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()),
        "Failed to enumerate Instance Layer Properties.");

    // collect all layers that are not available inside this error message
    std::string errorMessage = "";

    for(const char* layerName : layers) {
        bool layerFound = false;

        for(const VkLayerProperties& layerProperties : availableLayers) {
            if(strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if(!layerFound) {
            errorMessage += "    " + std::string(layerName) + "\n";
        }
    }

    if(!errorMessage.empty()) {
        errorMessage = "The following instance layers were requested but not available:\n"
                       + errorMessage;
        throw InstanceCreationException(errorMessage);
    }
}

void InstanceBuilder::assertExtensionSupport(const std::vector<const char*> extensions) const {
    uint32_t extensionCount;
    check<InstanceCreationException>(
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr),
        "Failed to enumerate Instance Extension Properties.");

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    check<InstanceCreationException>(
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               availableExtensions.data()),
        "Failed to enumerate Instance Extension Properties.");

    // collect all extensions that are not available inside this error message
    std::string errorMessage;

    for(const char* extensionName : extensions) {
        bool extensionFound = false;

        for(const VkExtensionProperties& extensionProperties : availableExtensions) {
            if(strcmp(extensionName, extensionProperties.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }

        if(!extensionFound) {
            errorMessage += "    " + std::string(extensionName) + "\n";
        }
    }

    if(!errorMessage.empty()) {
        errorMessage = "The following instance extensions were requested but not available:\n"
                       + errorMessage;
        throw InstanceCreationException(errorMessage);
    }
}
