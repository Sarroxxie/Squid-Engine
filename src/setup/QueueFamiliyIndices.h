#pragma once

#include <optional>
#include <vulkan/vulkan.h>

/*
If different Queue Families are required for your project, you can add them to
the QueueFamilyIndices struct and update the "isComplete()" and
"findQueueFamilies(..)" methods. As both of those methods are used to determine
whether a Physical Device is suitable, they must always be in sync.
*/

/*
Contains the indices to different queue families for later referencing.
*/
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    // Checks if all the indices are set.
    bool isComplete();
};

namespace QueueFamilyFinder {
// Attempts to find the queue family indices to a queue family that supports the
// families listed in the struct "QueueFamilyIndices". If you want to see if
// they are support, call "isComplete()" on the return value.
QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& physicalDevice);
}  // namespace QueueFamilyFinder