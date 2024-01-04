#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/*
Wrapper for creation of a VkInstance.
*/
class InstanceBuilder
{
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

    // Requests layers for the instance. Throws an exception if at least one of the Layers is not available.
    InstanceBuilder& requestLayers(const std::vector<const char*>& layers);

    // Requests extensions for the instance. Throws an exception if at least one
    // of the Extensions is not available.
    InstanceBuilder& requestExtensions(const std::vector<const char*>& extensions);

    // Attaches a Debug Utils Messenger to the instance for debugging its creation and destruction.
    InstanceBuilder& attachDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& messengerCreateInfo);

    // Creates a VkInstance. Throws an exception when creation failed.
    void build(VkInstance& instance);

  private:
    VkApplicationInfo    applicationInfo;
    VkInstanceCreateInfo createInfo;

    // Checks if the added layers are supported by the GPU. Throws an
    // exception if an extension is not supported.
    void assertLayerSupport(std::vector<const char*> layers);

    // Checks if the added extensions are supported by the GPU. Throws an
    // exception if an extension is not supported.
    void assertExtensionSupport(std::vector<const char*> extensions);
};