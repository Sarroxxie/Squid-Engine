#pragma once

#include <vulkan/vulkan.h>

class InstanceBuilder
{
  private:
    VkApplicationInfo    applicationInfo;
    VkInstanceCreateInfo createInfo;

  public:
    InstanceBuilder();
    // Sets the name of the application. Default name is "Application" if no name is provided.
    InstanceBuilder& setAppName(const char* applicationName);

    // Sets the version of the application. Version should be constructed with
    // VK_MAKE_VERSION. Default version is 1.0.0.
    InstanceBuilder& setAppVersion(uint32_t version);
    // Sets the version of the application. Default version is 1.0.0.
    InstanceBuilder& setAppVersion(uint32_t major, uint32_t minor, uint32_t patch);

    // Sets the name of the engine. Default name is "Engine" if no name is provided.
    InstanceBuilder& setEngineName(const char* engineName);

    // Sets the version of the engine. Version should be constructed with
    // VK_MAKE_VERSION. Default version is 1.0.0.
    InstanceBuilder& setEngineVersion(uint32_t version);
    // Sets the version of the engine. Default version is 1.0.0.
    InstanceBuilder& setEngineVersion(uint32_t major, uint32_t minor, uint32_t patch);

    // Sets the version of the Vulkan API. Version should be constructed with
    // VK_MAKE_API_VERSION. Default version is 0.1.3.
    InstanceBuilder& setApiVersion(uint32_t version);

    // Sets the version of the Vulkan API. Default version is 0.1.3.
    InstanceBuilder& setApiVersion(uint32_t variant, uint32_t major, uint32_t minor);

    // TODO: validation layer + extensions

    // Creates a VkInstance.
    VkResult build(VkInstance& instance);
};