#include "PhysicalDeviceSelector.h"

#include "QueueFamiliyIndices.h"
#include <stdexcept>
#include <vector>
#include <iostream>

VkPhysicalDevice& PhysicalDeviceSelector::selectPhysicalDevice(VkInstance& instance) {
    VkPhysicalDevice physicalDevice      = VK_NULL_HANDLE;
    int              physicalDeviceScore = -1;
    uint32_t         deviceCount         = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0) {
        throw std::runtime_error("No available GPU supports Vulkan!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for(const auto& device : devices) {
        if(rateDeviceSuitability(device) > physicalDeviceScore) {
            physicalDevice = device;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("No available GPU found!");
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    std::cout << "Using Physical Device \"" << deviceProperties.deviceName << "\"\n";

    return physicalDevice;
}

int PhysicalDeviceSelector::rateDeviceSuitability(const VkPhysicalDevice& device) {
    QueueFamilyIndices indices = QueueFamilyFinder::findQueueFamilies(device);
    if(indices.isComplete()) {
        return 0;
    }
    return -1;
}
