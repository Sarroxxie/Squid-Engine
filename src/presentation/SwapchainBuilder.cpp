#include "SwapchainBuilder.h"

#include "output/VulkanCheck.h"
#include "output/Logger.h"

SwapchainBuilder::SwapchainBuilder() {}

SwapchainBuilder::SwapchainBuilder(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
    : physicalDevice(physicalDevice) {
    setPhysicalDevice(physicalDevice);
    setSurface(surface);
}

// TODO: this should also have an option to return false!
SwapchainBuilder& SwapchainBuilder::setPhysicalDevice(const VkPhysicalDevice& physicalDevice) {
    check(physicalDevice != VK_NULL_HANDLE, "Physical Device is invalid on Swapchain creation.");
    this->physicalDevice = physicalDevice;
    return *this;
}

void SwapchainBuilder::build(const VkDevice& device, VkSwapchainKHR& swapchain) const {
    // TODO move all checks into this function! else every single setter would
    // have to be surrounded by try/catch

    check(physicalDevice != VK_NULL_HANDLE, "Physical Device is invalid on Swapchain creation.");
    check(swapchainCreateInfo.surface != VK_NULL_HANDLE,
          "Surface is invalid on Swapchain creation.");
    check(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain),
          "Failed to create Swapchain.");
    SLOG_INFO("Successfully created Swapchain.");
}

SwapchainBuilder& SwapchainBuilder::setFlags(const VkSwapchainCreateFlagsKHR flags) {
    swapchainCreateInfo.flags = flags;
    return *this;
}

// TODO: this should also have an option to return false!
SwapchainBuilder& SwapchainBuilder::setSurface(const VkSurfaceKHR surface) {
    // TODO: should we even check this here? -> yes, because we have to requery stuff....
    check(surface != VK_NULL_HANDLE, "Surface is invalid on Swapchain creation.");
    swapchainCreateInfo.surface = surface;

    // need to (re-)query
    querySurfaceCapabilities();
    querySurfaceFormats();
    querySurfacePresentModes();
    
    // adjust parameters to new surface
    setImageExtent(surfaceCapabilities.currentExtent);
    if(swapchainCreateInfo.minImageCount < surfaceCapabilities.minImageCount)
        setMinImageCount(surfaceCapabilities.minImageCount);
    
    check(checkCreateInfoSupport(), "Surface is lacking support for requested features.");

    return *this;
}

SwapchainBuilder& SwapchainBuilder::setMinImageCount(const uint32_t minImageCount,
                                                     bool& success) {
    if(checkMinImageCountSupport(minImageCount)) {
        swapchainCreateInfo.minImageCount = minImageCount;
        success                           = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat,
                                                     bool& success) {
    if(checkSurfaceFormatSupport(surfaceFormat)) {
        swapchainCreateInfo.imageFormat     = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        success                             = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageExtent(const VkExtent2D imageExtent,
                                                   bool&            success) {
    if(checkImageExtentSupport(imageExtent)) {
        swapchainCreateInfo.imageExtent = imageExtent;
        success                         = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageArrayLayers(const uint32_t imageArrayLayers,
                                                        bool& success) {
    if(checkImageArrayLayersSupport(imageArrayLayers)) {
        swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
        success                              = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageUsage(const VkImageUsageFlags imageUsage,
                                                  bool& success) {
    if(checkImageUsageSupport(imageUsage)) {
        swapchainCreateInfo.imageUsage = imageUsage;
        success                        = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setImageSharingMode(const VkSharingMode imageSharingMode) {
    swapchainCreateInfo.imageSharingMode = imageSharingMode;
    return *this;
}

// TODO: check if we should instead copy the data and store it inside the
// swapchain builder to avoid it going out of scope
SwapchainBuilder& SwapchainBuilder::setQueueFamilyIndices(const std::vector<uint32_t> queueFamilyIndices) {
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
    swapchainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices.data();
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform,
                                                    bool& success) {
    if(checkPreTransformSupport(preTransform)) {
        swapchainCreateInfo.preTransform = preTransform;
        success                          = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha, bool& success) {
    if(checkCompositeAlphaSupport(compositeAlpha)) {
        swapchainCreateInfo.compositeAlpha = compositeAlpha;
        success                          = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setPresentMode(const VkPresentModeKHR presentMode, bool& success) {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode)
       != surfacePresentModes.end()) {
        swapchainCreateInfo.presentMode = presentMode;
        success = true;
    } else {
        success = false;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setClipped(const VkBool32 clipped) {
    swapchainCreateInfo.clipped = clipped;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setOldSwapchain(const VkSwapchainKHR oldSwapchain) {
    swapchainCreateInfo.oldSwapchain = oldSwapchain;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo, bool& success) {
    success = swapchainCreateInfo.surface != VK_NULL_HANDLE;
    if(!success) {
        SLOG_ERROR("Surface is invalid when attempting to set SwapchainCreateInfo.");
        return *this;
    } else {
        this->swapchainCreateInfo = swapchainCreateInfo;
        // need to re-query
        querySurfaceCapabilities();
        querySurfaceFormats();
        querySurfacePresentModes();

        check(checkCreateInfoSupport(), "Surface is lacking support for requested features.");
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::enableVsyncPresentMode(bool& success) {
    // TODO: test what happens if FIFO is active and minImageCount = 1! ->
    // concern is naming here: vsync requires more than 1 image at a time but the
    // double buffering function requires the present mode to be set correctly

    if(swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_MAILBOX_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_RELAXED_KHR
       && swapchainCreateInfo.presentMode != VK_PRESENT_MODE_FIFO_LATEST_READY_KHR) {
        setPresentMode(VK_PRESENT_MODE_FIFO_KHR, success);
        if(!success) {
            SLOG_ERROR("Failed to set Swapchain present mode to VK_PRESENT_MODE_FIFO_KHR when attempting to enable Vsync.");
        } else {
            SLOG_INFO("Set Swapchain present mode to VK_PRESENT_MODE_FIFO_KHR when enabling Vsync.");
        }
    } else {
        success = true;
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setToDoubleBuffering(bool& success) {
    setMinImageCount(2, success);
    if(!success) {
        SLOG_ERROR("Failed to set Swapchain minImageCount to 2 when attempting to enable double buffering.");
        return *this;
    } else {
        SLOG_INFO("Set Swapchain minImageCount to 2 to enable double buffering.");
        enableVsyncPresentMode(success);
    }
    return *this;
}

SwapchainBuilder& SwapchainBuilder::setToTripleBuffering(bool& success) {
    setMinImageCount(2, success);
    if(!success) {
        SLOG_ERROR("Failed to set Swapchain minImageCount to 3 when attempting to enable triple buffering.");
    } else {
        SLOG_INFO("Set Swapchain minImageCount to 3 to enable triple buffering.");
        enableVsyncPresentMode();
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
