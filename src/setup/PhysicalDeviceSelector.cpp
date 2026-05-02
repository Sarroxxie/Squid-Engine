#include "PhysicalDeviceSelector.h"

#include "QueueFamiliyIndices.h"
#include "output/VulkanCheck.h"
#include <stdexcept>
#include <vector>
#include <iostream>

VkPhysicalDevice& PhysicalDeviceSelector::selectPhysicalDevice(VkInstance& instance,
                                                               const VkSurfaceKHR& surface) {
    VkPhysicalDevice physicalDevice      = VK_NULL_HANDLE;
    int              physicalDeviceScore = -1;
    uint32_t         deviceCount         = 0;
    check(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr),
          "Failed to enumerate Physical Devices.");

    check(deviceCount != 0, "No available GPU supports Vulkan.");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    check(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()),
          "Failed to enumerate Physical Devices.");

    for(const auto& device : devices) {
        int currentScore = rateDeviceSuitability(device, surface);
        if(currentScore > physicalDeviceScore) {
            physicalDevice = device;
            physicalDeviceScore = currentScore;
        }
    }

    check(physicalDevice != VK_NULL_HANDLE, "No suitable GPU found.");

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    // TODO: should use a logger here!
    std::cout << "Using Physical Device \"" << deviceProperties.deviceName << "\"\n";

    return physicalDevice;
}

int PhysicalDeviceSelector::rateDeviceSuitability(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
    int score = 0;

    // these queue families are required
    QueueFamilyIndices indices = QueueFamilyUtils::findQueueFamilies(device, surface);
    if(!indices.isComplete()) {
        return -1;
    }

    // weighs dedicated GPUs higher
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1;
    }

    return score;
}
