#include "DefaultPhysicalDeviceSelector.h"

int DefaultPhysicalDeviceSelector::rateDeviceSuitability(const VkPhysicalDevice& device) {
    int score = PhysicalDeviceSelector::rateDeviceSuitability(device);
    // The score is only negative if the required queue family indices are not found.
    if(score < 0)
        return -1;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 100;
    }

    return score;
}