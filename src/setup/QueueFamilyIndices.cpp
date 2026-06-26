#include "QueueFamiliyIndices.h"


#include "output/VulkanCheck.h"
#include <vector>
#include <set>

bool QueueFamilyIndices::isComplete() const {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

QueueFamilyIndices QueueFamilyUtils::findQueueFamilies(const VkPhysicalDevice& physicalDevice,
                                                       const VkSurfaceKHR& surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                             queueFamilies.data());

    for(uint32_t i = 0; i < queueFamilyCount; i++) {
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        // query for window surface support
        VkBool32 surfaceSupport = false;
        check(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &surfaceSupport),
              "Failed to get Physical Device Surface Support.");
        if(surfaceSupport) {
            indices.presentFamily = i;
        }
    }
    return indices;
}

std::vector<VkDeviceQueueCreateInfo> QueueFamilyUtils::toQueueCreateInfos(
    QueueFamilyIndices& indices,
    const float* const  queuePriority) {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    // For each unique index, we create a QueueCreateInfo. If we had multiple
    // QueueCreateInfos for the same index, Vulkan would crash.
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                              indices.presentFamily.value()};

    for(uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    return queueCreateInfos;
}
