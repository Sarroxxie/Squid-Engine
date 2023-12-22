#include "InstanceBuilder.h"

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

VkResult InstanceBuilder::build(VkInstance& instance) {
    return vkCreateInstance(&createInfo, nullptr, &instance);
}
