#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/*
Wrapper for creation of a VkDevice. A graphics queue is added by default (with
highest priority), which can be discarded by using "setQueues(..)".
*/
class DeviceBuilder
{
  public:
    // Adds a graphics queue by default (with highest priority). If this is
    // unwanted, use "setQueues(..)" for full control.
    DeviceBuilder(VkPhysicalDevice& physicalDevice);

    // Sets flags for device creation.
    DeviceBuilder& setFlags(const VkDeviceCreateFlags flags);

    // Sets features for device creation.
    DeviceBuilder& setFeatures(const VkPhysicalDeviceFeatures& features);

    // Requests extensions for the device. Throws an exception if at least one
    // of the Extensions is not available.
    DeviceBuilder& requestExtensions(const std::vector<const char*> extensions);

    DeviceBuilder& addQueue(VkDeviceQueueCreateInfo queueCreateInfo);
    DeviceBuilder& addQueues(const std::vector<VkDeviceQueueCreateInfo> queueCreateInfos);

    // Sets queues for device creation. This will discard every previously added queue.
    DeviceBuilder& setQueues(const std::vector<VkDeviceQueueCreateInfo> queueCreateInfos);

    // Creates a VkDevice. Throws an exception when creation failed.
    void build(VkDevice& device);

  private:
    const VkPhysicalDevice               physicalDevice;
    VkDeviceCreateInfo                   deviceCreateInfo;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // Checks if the added extensions are supported by the GPU. Throws an
    // exception if an extension is not supported.
    void assertExtensionSupport(std::vector<const char*> extensions);
};