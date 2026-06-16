#include "SwapchainBuilder.h"

#include "output/VulkanCheck.h"
#include "output/Logger.h"
#include <algorithm>

SwapchainBuilder::SwapchainBuilder() {}

SwapchainBuilder::SwapchainBuilder(const VkPhysicalDevice& physicalDevice,
                                   const VkSurfaceKHR&     surface) {
    setPhysicalDevice(physicalDevice);
    setSurface(surface);
}

// TODO: finish implementing this
Swapchain SwapchainBuilder::build(const VkDevice& device) const {
    Swapchain swapchain;
    check(physicalDevice != VK_NULL_HANDLE, "Physical Device is invalid on Swapchain creation.");
    check(swapchainCreateInfo.surface != VK_NULL_HANDLE,
          "Surface is invalid on Swapchain creation.");
    check(device != VK_NULL_HANDLE, "Device is invalid on Swapchain creation.");

    check(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain.handle),
          "Failed to create Swapchain.");
    SLOG_INFO("Successfully created Swapchain.");

    // extracting swapchain images
    uint32_t swapchainImageCount;
    // TODO: need to insert "check" functions here!
    vkGetSwapchainImagesKHR(device, swapchain.handle, &swapchainImageCount, nullptr);
    swapchain.images.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(device, swapchain.handle, &swapchainImageCount,
                            swapchain.images.data());
    swapchain.imageFormat = swapchainCreateInfo.imageFormat;
    swapchain.extent      = swapchainCreateInfo.imageExtent;

    return swapchain;
}

SwapchainBuilder& SwapchainBuilder::setPhysicalDevice(const VkPhysicalDevice& physicalDevice,
                                                      bool& success) {
    success = physicalDevice != VK_NULL_HANDLE;
    if(success) {
        this->physicalDevice = physicalDevice;
        // for querying we need a valid surface
        if(swapchainCreateInfo.surface != VK_NULL_HANDLE) {
            // need to (re-)query
            querySurfaceCapabilities();
            querySurfaceFormats();
            querySurfacePresentModes();

            // adjust settings to newly queried capabilities
            setImageExtent(surfaceCapabilities.currentExtent);
            if(swapchainCreateInfo.minImageCount < surfaceCapabilities.minImageCount)
                setMinImageCount(surfaceCapabilities.minImageCount);

            bool isSupportComplete = checkCreateInfoSupport();
            if(!isSupportComplete) {
                SLOG_WARNING("Current settings are not compatible with new Physical Device. Reverting to default settings.");
                setToDefaultSettings();
            }
        }
    } else {
        SLOG_WARNING("Attempted to set Physical Device to a NULL object on Swapchain creation. Reverting to previously used Physical Device");
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setPhysicalDevice(const VkPhysicalDevice& physicalDevice) {
    bool success;
    return setPhysicalDevice(physicalDevice, success);
}

SwapchainBuilder& SwapchainBuilder::setSurface(const VkSurfaceKHR surface, bool& success) {
    success = surface != VK_NULL_HANDLE;
    if(success) {
        swapchainCreateInfo.surface = surface;
        // for querying we need a valid physical device
        if(physicalDevice != VK_NULL_HANDLE) {
            // need to (re-)query
            querySurfaceCapabilities();
            querySurfaceFormats();
            querySurfacePresentModes();

            // adjust settings to newly queried capabilities
            setImageExtent(surfaceCapabilities.currentExtent);
            if(swapchainCreateInfo.minImageCount < surfaceCapabilities.minImageCount)
                setMinImageCount(surfaceCapabilities.minImageCount);

            bool isSupportComplete = checkCreateInfoSupport();
            if(!isSupportComplete) {
                SLOG_WARNING("Attempted to set Surface but it does not support requested features. Reverting to default settings.");
                setToDefaultSettings();
            }
        }
    } else {
        SLOG_WARNING("Attempted to set Surface to a NULL object on Swapchain creation. Reverting to previously used surface.");
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setSurface(const VkSurfaceKHR surface) {
    bool success;
    return setSurface(surface, success);
}

SwapchainBuilder& SwapchainBuilder::setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo,
                                                           bool& success) {
    success = swapchainCreateInfo.surface != VK_NULL_HANDLE;
    if(success) {
        // as these settings don't need to be checked for support, we can set them immediately
        setSurface(swapchainCreateInfo.surface);
        setFlags(swapchainCreateInfo.flags);
        setClipped(swapchainCreateInfo.clipped);
        setImageSharingMode(swapchainCreateInfo.imageSharingMode);
        if(swapchainCreateInfo.queueFamilyIndexCount != 0) {
            std::vector<uint32_t> queueFamilyIndices =
                std::vector<uint32_t>(swapchainCreateInfo.pQueueFamilyIndices,
                                      swapchainCreateInfo.pQueueFamilyIndices
                                          + swapchainCreateInfo.queueFamilyIndexCount);
            setQueueFamilyIndices(queueFamilyIndices);
        }
        setOldSwapchain(swapchainCreateInfo.oldSwapchain);

        if(physicalDevice != VK_NULL_HANDLE) {
            // this means that we have already queried surface properties
            setSurfaceFormat(VkSurfaceFormatKHR{swapchainCreateInfo.imageFormat,
                                                swapchainCreateInfo.imageColorSpace});
            setMinImageCount(swapchainCreateInfo.minImageCount);
            setImageExtent(surfaceCapabilities.currentExtent);
            setImageArrayLayers(swapchainCreateInfo.imageArrayLayers);
            setImageUsage(swapchainCreateInfo.imageUsage);
            setPreTransform(swapchainCreateInfo.preTransform);
            setCompositeAlpha(swapchainCreateInfo.compositeAlpha);
            setPresentMode(swapchainCreateInfo.presentMode);
        } else {
            // we couldn't yet query surface properties (because no physical
            // device is set yet) so we will just set the values and check if
            // they are supported when setting the physical device
            this->swapchainCreateInfo.imageFormat = swapchainCreateInfo.imageFormat;
            this->swapchainCreateInfo.minImageCount = swapchainCreateInfo.minImageCount;
            this->swapchainCreateInfo.imageExtent = swapchainCreateInfo.imageExtent;
            this->swapchainCreateInfo.imageArrayLayers = swapchainCreateInfo.imageArrayLayers;
            this->swapchainCreateInfo.imageUsage = swapchainCreateInfo.imageUsage;
            this->swapchainCreateInfo.preTransform = swapchainCreateInfo.preTransform;
            this->swapchainCreateInfo.compositeAlpha = swapchainCreateInfo.compositeAlpha;
            this->swapchainCreateInfo.presentMode = swapchainCreateInfo.presentMode;
        }
    } else {
        SLOG_WARNING("New Surface is invalid when attempting to set SwapchainCreateInfo. Reverting to previously used settings.");
        // no need to do anything as nothing has been updated yet
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo) {
    bool success;
    return setSwapchainCreateInfo(swapchainCreateInfo, success);
}

SwapchainBuilder& SwapchainBuilder::setFlags(const VkSwapchainCreateFlagsKHR flags) {
    swapchainCreateInfo.flags = flags;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setMinImageCount(const uint32_t minImageCount,
                                                     bool& success) {
    success = checkMinImageCountSupport(minImageCount);
    if(success) {
        swapchainCreateInfo.minImageCount = minImageCount;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setMinImageCount(const uint32_t minImageCount) {
    bool success;
    return setMinImageCount(minImageCount, success);
}

SwapchainBuilder& SwapchainBuilder::setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat,
                                                     bool& success) {
    success = checkSurfaceFormatSupport(surfaceFormat);
    if(success) {
        swapchainCreateInfo.imageFormat     = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat) {
    bool success;
    return setSurfaceFormat(surfaceFormat, success);
}

SwapchainBuilder& SwapchainBuilder::setImageExtent(const VkExtent2D imageExtent,
                                                   bool&            success) {
    success = checkImageExtentSupport(imageExtent);
    if(success) {
        swapchainCreateInfo.imageExtent = imageExtent;
    } else if(imageExtent.width == std::numeric_limits<uint32_t>::max()) {
        // according to the Vulkan Tutorial these special values can occur with
        // some window managers and need to be handled accordingsly
        swapchainCreateInfo.imageExtent = VkExtent2D{
            std::clamp<uint32_t>(imageExtent.width,
                                 surfaceCapabilities.minImageExtent.width,
                                 surfaceCapabilities.maxImageExtent.width),
            std::clamp<uint32_t>(imageExtent.height,
                                 surfaceCapabilities.minImageExtent.height,
                                 surfaceCapabilities.maxImageExtent.height)};
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageExtent(const VkExtent2D imageExtent) {
    bool success;
    return setImageExtent(imageExtent, success);
}

SwapchainBuilder& SwapchainBuilder::setImageArrayLayers(const uint32_t imageArrayLayers,
                                                        bool& success) {
    success = checkImageArrayLayersSupport(imageArrayLayers);
    if(success) {
        swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageArrayLayers(const uint32_t imageArrayLayers) {
    bool success;
    return setImageArrayLayers(imageArrayLayers, success);
}

SwapchainBuilder& SwapchainBuilder::setImageUsage(const VkImageUsageFlags imageUsage,
                                                  bool& success) {
    success = checkImageUsageSupport(imageUsage);
    if(success) {
        swapchainCreateInfo.imageUsage = imageUsage;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageUsage(const VkImageUsageFlags imageUsage) {
    bool success;
    return setImageUsage(imageUsage, success);
}

SwapchainBuilder& SwapchainBuilder::setImageSharingMode(const VkSharingMode imageSharingMode) {
    swapchainCreateInfo.imageSharingMode = imageSharingMode;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setQueueFamilyIndices(const std::vector<uint32_t> queueFamilyIndices) {
    // copy the queueFamilyIndices to avoid them going out of scope before "build" has been called
    this->queueFamilyIndices = std::vector<uint32_t>(queueFamilyIndices);
    swapchainCreateInfo.queueFamilyIndexCount = this->queueFamilyIndices.size();
    swapchainCreateInfo.pQueueFamilyIndices   = this->queueFamilyIndices.data();
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform,
                                                    bool& success) {
    success = checkPreTransformSupport(preTransform);
    if(success) {
        swapchainCreateInfo.preTransform = preTransform;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform) {
    bool success;
    return setPreTransform(preTransform, success);
}

SwapchainBuilder& SwapchainBuilder::setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha,
                                                      bool& success) {
    success = checkCompositeAlphaSupport(compositeAlpha);
    if(success) {
        swapchainCreateInfo.compositeAlpha = compositeAlpha;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha) {
    bool success;
    return setCompositeAlpha(compositeAlpha, success);
}

SwapchainBuilder& SwapchainBuilder::setPresentMode(const VkPresentModeKHR presentMode,
                                                   bool& success) {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode)
       != surfacePresentModes.end()) {
        swapchainCreateInfo.presentMode = presentMode;
        success                         = true;
    } else {
        success = false;
        SLOG_WARNING("Requested Swapchain presentMode not supported. (may also be caused by either invalid Physical Device or Surface)");
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setPresentMode(const VkPresentModeKHR presentMode) {
    bool success;
    return setPresentMode(presentMode, success);
}

SwapchainBuilder& SwapchainBuilder::setClipped(const VkBool32 clipped) {
    swapchainCreateInfo.clipped = clipped;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setOldSwapchain(const VkSwapchainKHR oldSwapchain) {
    swapchainCreateInfo.oldSwapchain = oldSwapchain;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::enableVsyncPresentMode(bool& success) {
    if(swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_MAILBOX_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_RELAXED_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_LATEST_READY_KHR) {
        setPresentMode(VK_PRESENT_MODE_FIFO_KHR, success);
        if(!success) {
            SLOG_WARNING("Failed to set Swapchain present mode to VK_PRESENT_MODE_FIFO_KHR when attempting to enable Vsync.");
        } else {
            SLOG_INFO("Set Swapchain present mode to VK_PRESENT_MODE_FIFO_KHR when enabling Vsync.");
        }
    } else {
        success = true;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::enableVsyncPresentMode() {
    bool success;
    return enableVsyncPresentMode(success);
}

SwapchainBuilder& SwapchainBuilder::setToDoubleBuffering(bool& success) {
    setMinImageCount(2, success);
    if(success) {
        SLOG_INFO("Set Swapchain minImageCount to 2 to enable double buffering.");
        enableVsyncPresentMode(success);
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setToDoubleBuffering() {
    bool success;
    return setToDoubleBuffering(success);
}

SwapchainBuilder& SwapchainBuilder::setToTripleBuffering(bool& success) {
    setMinImageCount(3, success);
    if(success) {
        SLOG_INFO("Set Swapchain minImageCount to 3 to enable triple buffering.");
        enableVsyncPresentMode(success);
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setToTripleBuffering() {
    bool success;
    return setToTripleBuffering(success);
}

SwapchainBuilder& SwapchainBuilder::setToDefaultSettings() {
    // need to store and set surface again as there does not exists a default option for this
    VkSurfaceKHR surface        = swapchainCreateInfo.surface;
    swapchainCreateInfo         = defaultSwapchainCreateInfo;
    swapchainCreateInfo.surface = surface;

    if(physicalDevice != VK_NULL_HANDLE && swapchainCreateInfo.surface != VK_NULL_HANDLE) {
        setMinImageCount(surfaceCapabilities.minImageCount);
        setImageExtent(surfaceCapabilities.currentExtent);
    }

    return *this;
}


void SwapchainBuilder::querySurfaceCapabilities() {
    check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
              physicalDevice, swapchainCreateInfo.surface, &surfaceCapabilities),
          "Failed to query Surface Capabilities on Swapchain Creation.");
}

void SwapchainBuilder::querySurfaceFormats() {
    uint32_t surfaceFormatCount;
    check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                               swapchainCreateInfo.surface,
                                               &surfaceFormatCount, nullptr),
          "Failed to query Surface Formats on Swapchain Creation.");
    surfaceFormats.resize(surfaceFormatCount);
    check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                               swapchainCreateInfo.surface, &surfaceFormatCount,
                                               surfaceFormats.data()),
          "Failed to query Surface Formats on Swapchain Creation.");
}

void SwapchainBuilder::querySurfacePresentModes() {
    uint32_t presentModeCount;
    check(vkGetPhysicalDeviceSurfacePresentModesKHR(
              physicalDevice, swapchainCreateInfo.surface, &presentModeCount, nullptr),
          "Failed to query Surface Present Modes on Swapchain Creation.");
    surfacePresentModes.resize(presentModeCount);
    check(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                    swapchainCreateInfo.surface, &presentModeCount,
                                                    surfacePresentModes.data()),
          "Failed to query Surface Present Modes on Swapchain Creation.");
}

bool SwapchainBuilder::checkCreateInfoSupport() const {
    bool success = true;

    success = success && checkMinImageCountSupport(swapchainCreateInfo.minImageCount);
    success = success
              && checkSurfaceFormatSupport(VkSurfaceFormatKHR{
                  swapchainCreateInfo.imageFormat, swapchainCreateInfo.imageColorSpace});
    success = success && checkImageExtentSupport(swapchainCreateInfo.imageExtent);
    success = success && checkImageArrayLayersSupport(swapchainCreateInfo.imageArrayLayers);
    success = success && checkImageUsageSupport(swapchainCreateInfo.imageUsage);
    success = success && checkPreTransformSupport(swapchainCreateInfo.preTransform);
    success = success && checkCompositeAlphaSupport(swapchainCreateInfo.compositeAlpha);
    success = success && checkPresentModeSupport(swapchainCreateInfo.presentMode);
    return success;
}

bool SwapchainBuilder::checkMinImageCountSupport(uint32_t minImageCount) const {
    if(minImageCount < surfaceCapabilities.minImageCount
       || minImageCount > surfaceCapabilities.maxImageCount) {
        SLOG_WARNING("Requested Swapchain minImageCount of "
                     << minImageCount << " not supported. Has to be between "
                     << surfaceCapabilities.minImageCount << " and "
                     << surfaceCapabilities.maxImageCount
                     << ". (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

// needed to check if a requested surface matches with a supported surface
bool operator==(const VkSurfaceFormatKHR& lhs, const VkSurfaceFormatKHR& rhs) {
    if(lhs.format == rhs.format && lhs.colorSpace == rhs.colorSpace)
        return true;
    return false;
}

bool SwapchainBuilder::checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat) const {
    if(std::find(surfaceFormats.begin(), surfaceFormats.end(), imageFormat)
       == surfaceFormats.end()) {

        SLOG_WARNING("Requested Swapchain Surface Format not supported. (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkImageExtentSupport(VkExtent2D imageExtent) const {
    surfaceCapabilities.minImageExtent;
    if(imageExtent.width < surfaceCapabilities.minImageExtent.width
       || imageExtent.width > surfaceCapabilities.maxImageExtent.width
       || imageExtent.height < surfaceCapabilities.minImageExtent.height
       || imageExtent.height > surfaceCapabilities.maxImageExtent.height) {
        SLOG_WARNING("Requested Swapchain imageExtent ("
                     << imageExtent.width << "," << imageExtent.height << ") not supported. Has to be between ("
                     << surfaceCapabilities.minImageExtent.width << ","
                     << surfaceCapabilities.minImageExtent.height << ") and ("
                     << surfaceCapabilities.maxImageExtent.width << ","
                     << surfaceCapabilities.maxImageExtent.height
                     << ").  (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkImageArrayLayersSupport(uint32_t imageArrayLayers) const {
    if(imageArrayLayers > surfaceCapabilities.maxImageArrayLayers) {
        SLOG_WARNING("Requested Swapchain imageArrayLayers of "
                     << imageArrayLayers << " not supported. Has to be lesser than or equal to "
                     << surfaceCapabilities.maxImageArrayLayers
                     << ". (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkImageUsageSupport(VkImageUsageFlags imageUsage) const {
    if((surfaceCapabilities.supportedUsageFlags | imageUsage)
       != surfaceCapabilities.supportedUsageFlags) {
        SLOG_WARNING("Requested Swapchain imageUsageFlags not supported. (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform) const {
    if((surfaceCapabilities.supportedTransforms | preTransform)
       != surfaceCapabilities.supportedTransforms) {
        SLOG_WARNING("Requested Swapchain preTransform not supported. (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha) const {
    if((surfaceCapabilities.supportedCompositeAlpha | compositeAlpha)
       != surfaceCapabilities.supportedCompositeAlpha) {
        SLOG_WARNING("Requested Swapchain compositeAlpha not supported. (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkPresentModeSupport(VkPresentModeKHR presentMode) const {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode)
       == surfacePresentModes.end()) {
        SLOG_WARNING("Requested Swapchain presentMode not supported. (may also be caused by either invalid Physical Device or Surface)");
        return false;
    }
    return true;
}
