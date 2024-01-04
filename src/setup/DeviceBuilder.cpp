#include "DeviceBuilder.h"

#include "QueueFamiliyIndices.h"
#include <stdexcept>
#include <string>

DeviceBuilder::DeviceBuilder(VkPhysicalDevice& physicalDevice)
    : physicalDevice(physicalDevice) {
    QueueFamilyIndices indices = QueueFamilyFinder::findQueueFamilies(physicalDevice);

    float queuePriority = 1.0f;
    // We always need a graphics queue to do graphics things.
    VkDeviceQueueCreateInfo graphicsQueueCreateInfo;
    graphicsQueueCreateInfo =
        VkDeviceQueueCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    graphicsQueueCreateInfo.queueCount       = 1;
    graphicsQueueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(graphicsQueueCreateInfo);

    deviceCreateInfo = VkDeviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
}

DeviceBuilder& DeviceBuilder::setFlags(const VkDeviceCreateFlags flags) {
    deviceCreateInfo.flags = flags;
    return *this;
}

DeviceBuilder& DeviceBuilder::setFeatures(const VkPhysicalDeviceFeatures& features) {
    deviceCreateInfo.pEnabledFeatures = &features;
    return *this;
}

DeviceBuilder& DeviceBuilder::requestExtensions(const std::vector<const char*> extensions) {
    if(extensions.size() == 0)
        return *this;
    deviceCreateInfo.enabledExtensionCount   = extensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    return *this;
}

DeviceBuilder& DeviceBuilder::addQueue(VkDeviceQueueCreateInfo queueCreateInfo) {
    this->queueCreateInfos.push_back(queueCreateInfo);
    return *this;
}

DeviceBuilder& DeviceBuilder::addQueues(const std::vector<VkDeviceQueueCreateInfo> queueCreateInfos) {
    if(queueCreateInfos.size() == 0)
        return *this;
    // append
    this->queueCreateInfos.insert(this->queueCreateInfos.end(),
                                  queueCreateInfos.begin(), queueCreateInfos.end());
    return *this;
}

DeviceBuilder& DeviceBuilder::setQueues(const std::vector<VkDeviceQueueCreateInfo> queueCreateInfos) {
    this->queueCreateInfos = queueCreateInfos;
    return *this;
}

void DeviceBuilder::build(VkDevice& device) {
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos    = queueCreateInfos.data();

    if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create VkDevice.");
    }
}

void DeviceBuilder::assertExtensionSupport(std::vector<const char*> extensions) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount,
                                         availableExtensions.data());

    // collect all extensions that are not available inside this error message
    std::string errorMessage;

    for(const char* extensionName : extensions) {
        bool extensionFound = false;

        for(const VkExtensionProperties& extensionProperties : availableExtensions) {
            if(strcmp(extensionName, extensionProperties.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }

        if(!extensionFound) {
            errorMessage += "    " + std::string(extensionName) + "\n";
        }
    }

    if(!errorMessage.empty()) {
        errorMessage = "The following device extensions were requested but not available:\n"
                       + errorMessage;
        throw std::runtime_error(errorMessage);
    }
}
