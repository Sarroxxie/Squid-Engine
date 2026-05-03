#include "DefaultPhysicalDeviceSelector.h"

int DefaultPhysicalDeviceSelector::rateDeviceSuitability(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) const{
    int score =
        PhysicalDeviceSelector::rateDeviceSuitability(device, surface);
    // The score is only negative if the required queue family indices are not found.
    if(score < 0)
        return -1;

    VkPhysicalDeviceProperties2 deviceProperties;
    vkGetPhysicalDeviceProperties2(device, &deviceProperties);
    VkPhysicalDeviceFeatures2 deviceFeatures;
    vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    if(deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 100;
    }

    return score;
}