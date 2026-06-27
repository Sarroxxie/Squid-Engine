#include "DeviceBuilder.h"

#include "output/VulkanCheck.h"
#include "output/Logger.h"
#include "Exceptions.h"

DeviceBuilder::DeviceBuilder(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
    : physicalDevice(physicalDevice)
    , surface(surface) {
    indices = QueueFamilyUtils::findQueueFamilies(physicalDevice, surface);

    // IMPORTANT: for this to work, the priority, which is referenced, must not
    // go out of scope until "build()" was called.
    queueCreateInfos =
        QueueFamilyUtils::toQueueCreateInfos(indices, &DEFAULT_QUEUE_PRIORITY);
}

DeviceBuilder& DeviceBuilder::setFlags(const VkDeviceCreateFlags flags) {
    deviceCreateInfo.flags = flags;
    return *this;
}

DeviceBuilder& DeviceBuilder::setFeatures(const VkPhysicalDeviceFeatures& features) {
    deviceCreateInfo.pEnabledFeatures = &features;
    return *this;
}

DeviceBuilder& DeviceBuilder::setFeatures(const VkPhysicalDeviceFeatures2& features) {
    deviceCreateInfo.pNext = &features;
    return *this;
}

DeviceBuilder& DeviceBuilder::requestExtensions(const std::vector<const char*>& extensions) {
    if(extensions.size() == 0)
        return *this;
    assertExtensionSupport(extensions);
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    return *this;
}

DeviceBuilder& DeviceBuilder::addQueue(const VkDeviceQueueCreateInfo queueCreateInfo) {
    this->queueCreateInfos.push_back(queueCreateInfo);
    return *this;
}

DeviceBuilder& DeviceBuilder::addQueues(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos) {
    if(queueCreateInfos.size() == 0)
        return *this;
    // append
    this->queueCreateInfos.insert(this->queueCreateInfos.end(),
                                  queueCreateInfos.begin(), queueCreateInfos.end());
    return *this;
}

DeviceBuilder& DeviceBuilder::setQueues(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos) {
    this->queueCreateInfos = queueCreateInfos;
    return *this;
}

QueueFamilyIndices DeviceBuilder::build(VkDevice& device) {
    deviceCreateInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

    check<DeviceCreationException>(vkCreateDevice(physicalDevice, &deviceCreateInfo,
                                                  nullptr, &device),
                                   "Failed to create VkDevice.");
    SLOG_INFO("Successfully created Logical Device.");
    return indices;
}

void DeviceBuilder::assertExtensionSupport(std::vector<const char*> extensions) const {
    uint32_t extensionCount;
    check<DeviceCreationException>(
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr),
        "Failed to enumerate Device Extension Properties.");

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    check<DeviceCreationException>(
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount,
                                             availableExtensions.data()),
        "Failed to enumerate Device Extension Properties.");

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
        throw DeviceCreationException(errorMessage);
    }
}
