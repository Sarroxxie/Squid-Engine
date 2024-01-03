#include "InstanceBuilder.h"
#include <stdexcept>

InstanceBuilder::InstanceBuilder() {
    // default data for instance creation
    applicationInfo = VkApplicationInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    applicationInfo.pApplicationName   = "Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName        = "Engine";
    applicationInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion         = VK_API_VERSION_1_3;

    createInfo = VkInstanceCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.pApplicationInfo = &applicationInfo;
}

InstanceBuilder& InstanceBuilder::setAppName(const char* applicationName) {
    if(!applicationName)
        return *this;
    applicationInfo.pApplicationName = applicationName;
    return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(uint32_t version) {
    applicationInfo.applicationVersion = version;
    return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(uint32_t major, uint32_t minor, uint32_t patch) {
    return setAppVersion(VK_MAKE_VERSION(major, minor, patch));
}

InstanceBuilder& InstanceBuilder::setEngineName(const char* engineName) {
    if(!engineName)
        return *this;
    applicationInfo.pEngineName = engineName;
    return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(uint32_t version) {
    applicationInfo.engineVersion = version;
    return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(uint32_t major, uint32_t minor, uint32_t patch) {
    return setEngineVersion(VK_MAKE_VERSION(major, minor, patch));
}

InstanceBuilder& InstanceBuilder::setApiVersion(uint32_t version) {
    applicationInfo.apiVersion = version;
    return *this;
}

InstanceBuilder& InstanceBuilder::setApiVersion(uint32_t variant, uint32_t major, uint32_t minor) {
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

void InstanceBuilder::build(VkInstance& instance) {
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create VkInstance.");
    }
}

void InstanceBuilder::assertLayerSupport(std::vector<const char*> layers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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
        throw std::runtime_error(errorMessage);
    }
}

void InstanceBuilder::assertExtensionSupport(std::vector<const char*> extensions) {
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                           availableExtensions.data());

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
        throw std::runtime_error(errorMessage);
    }
}
