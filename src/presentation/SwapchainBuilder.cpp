#include "SwapchainBuilder.h"

#include "output/VulkanCheck.h"
#include "output/Logger.h"

SwapchainBuilder::SwapchainBuilder() {}

SwapchainBuilder::SwapchainBuilder(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
    : physicalDevice(physicalDevice) {
    setPhysicalDevice(physicalDevice);
    setSurface(surface);
}

void SwapchainBuilder::setPhysicalDevice(const VkPhysicalDevice& physicalDevice) {
    check(false, "Physical Device is invalid on Swapchain creation.");
    //check(physicalDevice != VK_NULL_HANDLE, "Physical Device is invalid on Swapchain creation.");
    this->physicalDevice = physicalDevice;
}

void SwapchainBuilder::build(const VkDevice& device, VkSwapchainKHR& swapchain) const {
    check(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain),
          "Failed to create Swapchain.");
    SLOG_INFO("Successfully created Swapchain.");
}

void SwapchainBuilder::setFlags(const VkSwapchainCreateFlagsKHR flags) {
    swapchainCreateInfo.flags = flags;
}

void SwapchainBuilder::setSurface(const VkSurfaceKHR surface) {
    check(surface != VK_NULL_HANDLE, "Surface is invalid on Swapchain creation.");
    swapchainCreateInfo.surface = surface;

    // need to re-query
    querySurfaceCapabilities();
    querySurfaceFormats();
    querySurfacePresentModes();
    // TODO: need to check ALL the createInfo for support again here!
    setMinImageCount(surfaceCapabilities.minImageCount);
    setImageExtent(surfaceCapabilities.currentExtent);
}

bool SwapchainBuilder::setMinImageCount(const uint32_t minImageCount) {
    if(!checkMinImageCountSupport(minImageCount))
        return false;
    swapchainCreateInfo.minImageCount = minImageCount;
    return true;
}

bool SwapchainBuilder::setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat) {
    if(!checkSurfaceFormatSupport(surfaceFormat))
        return false;
    swapchainCreateInfo.imageFormat     = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    return true;
}

bool SwapchainBuilder::setImageExtent(const VkExtent2D imageExtent) {
    if(!checkImageExtentSupport(imageExtent))
        return false;
    swapchainCreateInfo.imageExtent = imageExtent;
    return true;
}

bool SwapchainBuilder::setImageArrayLayers(const uint32_t imageArrayLayers) {
    if(!checkImageArrayLayersSupport(imageArrayLayers))
        return false;
    swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
    return true;
}

bool SwapchainBuilder::setImageUsage(const VkImageUsageFlags imageUsage) {
    if(!checkImageUsageSupport(imageUsage))
        return false;
    swapchainCreateInfo.imageUsage = imageUsage;
    return true;
}

void SwapchainBuilder::setImageSharingMode(const VkSharingMode imageSharingMode) {
    swapchainCreateInfo.imageSharingMode = imageSharingMode;
}

// TODO: check if we should instead copy the data and store it inside the
// swapchain builder to avoid it going out of scope
void SwapchainBuilder::setQueueFamilyIndices(const std::vector<uint32_t> queueFamilyIndices) {
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
    swapchainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices.data();
}

bool SwapchainBuilder::setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform) {
    if(!checkPreTransformSupport(preTransform))
        return false;
    swapchainCreateInfo.preTransform = preTransform;
    return true;
}

bool SwapchainBuilder::setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha) {
    if(!checkCompositeAlphaSupport(compositeAlpha))
        return false;
    swapchainCreateInfo.compositeAlpha = compositeAlpha;
    return true;
}

bool SwapchainBuilder::setPresentMode(const VkPresentModeKHR presentMode) {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode)
       == surfacePresentModes.end()) {
        return false;
    }
    swapchainCreateInfo.presentMode = presentMode;
    return true;
}

void SwapchainBuilder::setClipped(const VkBool32 clipped) {
    swapchainCreateInfo.clipped = clipped;
}

void SwapchainBuilder::setOldSwapchain(const VkSwapchainKHR oldSwapchain) {
    swapchainCreateInfo.oldSwapchain = oldSwapchain;
}

bool SwapchainBuilder::setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo) {
    // TODO: need to set each field here and check on the way.... :(
    return false;
}

bool SwapchainBuilder::enableVsyncPresentMode() {
    // TODO: test what happens if FIFO is active and minImageCount = 1! ->
    // concern is naming here: vsync requires more than 1 image at a time but the
    // double buffering function requires the present mode to be set correctly

    if(swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_MAILBOX_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_RELAXED_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_LATEST_READY_KHR) {
        if(!setPresentMode(VK_PRESENT_MODE_FIFO_KHR)) {
            SLOG_ERROR("Failed to set Swapchain present mode to VK_PRESENT_MODE_FIFO_KHR when attempting to enable Vsync.");
            return false;
        }
        SLOG_INFO("Set Swapchain present mode to VK_PRESENT_MODE_FIFO_KHR when enabling Vsync.");
    }
    return true;
}

bool SwapchainBuilder::setToDoubleBuffering() {
    if(!setMinImageCount(2)) {
        SLOG_ERROR("Failed to set Swapchain minImageCount to 2 when attempting to enable double buffering.");
        return false;
    }
    SLOG_INFO("Set Swapchain minImageCount to 2 to enable double buffering.");
    return enableVsyncPresentMode();
}

bool SwapchainBuilder::setToTripleBuffering() {
    if(!setMinImageCount(3)) {
        SLOG_ERROR("Failed to set Swapchain minImageCount to 3 when attempting to enable triple buffering.");
        return false;
    }
    SLOG_INFO("Set Swapchain minImageCount to 3 to enable triple buffering.");
    return enableVsyncPresentMode();
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
    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
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
    std::vector<VkPresentModeKHR> presentModes;
    check(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                    swapchainCreateInfo.surface, &presentModeCount,
                                                    presentModes.data()),
          "Failed to query Surface Present Modes on Swapchain Creation.");
}

bool SwapchainBuilder::checkMinImageCountSupport(uint32_t minImageCount) const {
    if(minImageCount < surfaceCapabilities.minImageCount
       || minImageCount > surfaceCapabilities.maxImageCount) {
        SLOG_WARNING("Requested Swapchain minImageCount of "
                     << minImageCount << " not supported. Has to be between "
                     << surfaceCapabilities.minImageCount << " and "
                     << surfaceCapabilities.maxImageCount << ".");
        return false;
    }
    return true;
}

// TODO: should this be here in the code? we need this to iterate over the
// supported surface formats and compare if a requested format is supported
bool operator==(const VkSurfaceFormatKHR& lhs, const VkSurfaceFormatKHR& rhs) {
    if(lhs.format == rhs.format && lhs.colorSpace == rhs.colorSpace)
        return true;
    return false;
}

bool SwapchainBuilder::checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat) const {
    if(std::find(surfaceFormats.begin(), surfaceFormats.end(), imageFormat)
       == surfaceFormats.end()) {

        SLOG_WARNING("Requested Swapchain Surface Format not supported.");
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
                     << surfaceCapabilities.maxImageExtent.height << ").");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkImageArrayLayersSupport(uint32_t imageArrayLayers) const {
    if(imageArrayLayers > surfaceCapabilities.maxImageArrayLayers) {
        SLOG_WARNING("Requested Swapchain imageArrayLayers of "
                     << imageArrayLayers << " not supported. Has to be lesser than or equal to "
                     << surfaceCapabilities.maxImageArrayLayers << ".");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkImageUsageSupport(VkImageUsageFlags imageUsage) const {
    if((surfaceCapabilities.supportedUsageFlags | imageUsage)
       != surfaceCapabilities.supportedUsageFlags) {
        SLOG_WARNING("Requested Swapchain imageUsageFlags not supported.");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform) const {
    if((surfaceCapabilities.supportedTransforms | preTransform)
       != surfaceCapabilities.supportedTransforms) {
        SLOG_WARNING("Requested Swapchain preTransform not supported.");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha) const {
    if((surfaceCapabilities.supportedCompositeAlpha | compositeAlpha)
       != surfaceCapabilities.supportedCompositeAlpha) {
        SLOG_WARNING("Requested Swapchain compositeAlpha not supported.");
        return false;
    }
    return true;
}

bool SwapchainBuilder::checkPresentModeSupport(VkPresentModeKHR presentMode) const {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode)
       == surfacePresentModes.end()) {
        SLOG_WARNING("Requested Swapchain presentMode not supported.");
        return false;
    }
    return true;
}
