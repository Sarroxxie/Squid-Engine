#pragma once

#include <optional>
#include <vulkan/vulkan.h>
#include <vector>

/*
If different Queue Families are required for your project, you can add them to
the QueueFamilyIndices struct and update the "isComplete()" and
"findQueueFamilies(..)" methods. As both of those methods are used to determine
whether a Physical Device is suitable, they must always be in sync.
*/

/*
Contains the indices to different queue families for later referencing. To get
the corresponding VkQueues, use "vkGetDeviceQueue(..)".
*/
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    // Checks if all the indices are set.
    bool isComplete() const;
};

namespace QueueFamilyUtils {

// Attempts to find the queue family indices to a queue family that supports the
// families listed in the struct "QueueFamilyIndices". If you want to see if
// they are supported, call "isComplete()" on the return value.
QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& physicalDevice,
                                     const VkSurfaceKHR&     surface);

// Simplifies the process of creating QueueCreateInfos for the indices from QueueFamilyIndices.
std::vector<VkDeviceQueueCreateInfo> toQueueCreateInfos(QueueFamilyIndices& indices,
                                                        const float* const queuePriority);

}  // namespace QueueFamilyUtils