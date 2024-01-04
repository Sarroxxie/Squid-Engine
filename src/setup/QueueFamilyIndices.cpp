#include "QueueFamiliyIndices.h"

#include <vector>

bool QueueFamilyIndices::isComplete() {
    // TODO: reenable presentFamily check when window surface is implemented
    return graphicsFamily.has_value();  // && presentFamily.has_value();
}

QueueFamilyIndices QueueFamilyFinder::findQueueFamilies(const VkPhysicalDevice& physicalDevice) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                             queueFamilies.data());

    for(int i = 0; i < queueFamilyCount; i++) {
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        // TODO: query queueFamily for window surface support
    }
    return indices;
}