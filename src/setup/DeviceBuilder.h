#pragma once

#include "QueueFamiliyIndices.h"

/*
Wrapper for creation of a VkDevice. A graphics queue and a present queue are
added by default (with highest priority), which can be discarded by using
"setQueues(..)".
*/
class DeviceBuilder
{
  public:
    // Adds a graphics queue and a present queue by default (with highest
    // priority). If this is unwanted, use "setQueues(..)" for full control.
    DeviceBuilder(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

    // Sets flags for device creation.
    DeviceBuilder& setFlags(const VkDeviceCreateFlags flags);

    // Sets features for device creation. (legacy, use for Vulkan 1.0)
    DeviceBuilder& setFeatures(const VkPhysicalDeviceFeatures& features);

    // Sets features for device creation. (use this for Vulkan 1.1 and higher)
    DeviceBuilder& setFeatures(const VkPhysicalDeviceFeatures2& features);

    // Requests extensions for the device. Throws an exception if at least one
    // of the Extensions is not available.
    DeviceBuilder& requestExtensions(const std::vector<const char*>& extensions);

    // NOTE: VkDeviceQueueCreateInfo takes a float* as reference. Make sure that
    // the value it is referencing does not go out of scope until "build()" was called.
    DeviceBuilder& addQueue(const VkDeviceQueueCreateInfo queueCreateInfo);
    // NOTE: VkDeviceQueueCreateInfo takes a float* as reference. Make sure that
    // the value it is referencing does not go out of scope until "build()" was called.
    DeviceBuilder& addQueues(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos);

    // Sets queues for device creation. This will discard every previously added queue.
    // NOTE: VkDeviceQueueCreateInfo takes a float* as reference. Make sure that
    // the value it is referencing does not go out of scope until "build()" was called.
    DeviceBuilder& setQueues(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos);

    // Creates a VkDevice. Throws an exception when creation failed. Also
    // returns the indices to the created queues.
    QueueFamilyIndices build(VkDevice& device);

  private:
    const VkPhysicalDevice physicalDevice;
    const VkSurfaceKHR     surface;
    VkDeviceCreateInfo deviceCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    QueueFamilyIndices                   indices;

    // TODO: should this be moved somewhere else?
    const float DEFAULT_QUEUE_PRIORITY = 1.0f;

    // Checks if the added extensions are supported by the GPU. Throws an
    // exception if an extension is not supported.
    void assertExtensionSupport(std::vector<const char*> extensions) const;
};