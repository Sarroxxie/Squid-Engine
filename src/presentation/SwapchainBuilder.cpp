#include "SwapchainBuilder.h"

#include "output/VulkanCheck.h"

SwapchainBuilder::SwapchainBuilder() {}

SwapchainBuilder::SwapchainBuilder(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    : physicalDevice(physicalDevice) {
    // TODO: need to check that device and surface are valid + call query functions
    check(physicalDevice != VK_NULL_HANDLE, "Physical Device is invalid on Swapchain creation!");
    setSurface(surface);
}

void SwapchainBuilder::setPhysicalDevice(VkPhysicalDevice physicalDevice) {
    // TODO do we need to check if this is a valid physical device here?
    this->physicalDevice = physicalDevice;
}

void SwapchainBuilder::build(VkDevice device, VkSwapchainKHR* swapchain) {
    check(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, swapchain),
          "Failed to create Swapchain.");
}

void SwapchainBuilder::setFlags(VkSwapchainCreateFlagsKHR flags) {
    swapchainCreateInfo.flags = flags;
}

void SwapchainBuilder::setSurface(VkSurfaceKHR surface) {
    check((surface != VK_NULL_HANDLE), "Surface is invalid on Swapchain (re)creation!");
    swapchainCreateInfo.surface = surface;

    // need to re-query
    querySurfaceCapabilities();
    querySurfaceFormats();
    querySurfacePresentModes();
    // TODO: need to check ALL the createInfo for support again here!
    setMinImageCount(surfaceCapabilities.minImageCount);
    setImageExtent(surfaceCapabilities.currentExtent);
}

bool SwapchainBuilder::setMinImageCount(uint32_t minImageCount) {
    if(!checkMinImageCountSupport(minImageCount))
        return false;
    swapchainCreateInfo.minImageCount = minImageCount;
    return true;
}

bool SwapchainBuilder::setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat) {
    if(!checkSurfaceFormatSupport(surfaceFormat))
        return false;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    return true;
}

bool SwapchainBuilder::setImageExtent(VkExtent2D imageExtent) {
    if(!checkImageExtentSupport(imageExtent))
        return false;
    swapchainCreateInfo.imageExtent = imageExtent;
    return true;
}

bool SwapchainBuilder::setImageArrayLayers(uint32_t imageArrayLayers) {
    if(!checkImageArrayLayersSupport(imageArrayLayers))
        return false;
    swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
    return true;
}

bool SwapchainBuilder::setImageUsage(VkImageUsageFlags imageUsage) {
    if(!checkImageUsageSupport(imageUsage))
        return false;
    swapchainCreateInfo.imageUsage = imageUsage;
    return true;
}

void SwapchainBuilder::setImageSharingMode(VkSharingMode imageSharingMode) {
    swapchainCreateInfo.imageSharingMode = imageSharingMode;
}

// TODO: check if we should instead copy the data and store it inside the swapchain builder to avoid it going out of scope
void SwapchainBuilder::setQueueFamilyIndices(std::vector<uint32_t> queueFamilyIndices) {
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
    swapchainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices.data();
}

bool SwapchainBuilder::setPreTransform(VkSurfaceTransformFlagBitsKHR preTransform) {
    if(!checkPreTransformSupport(preTransform))
        return false;
    swapchainCreateInfo.preTransform = preTransform;
    return true;
}

bool SwapchainBuilder::setCompositeAlpha(VkCompositeAlphaFlagBitsKHR compositeAlpha) {
    if(!checkCompositeAlphaSupport(compositeAlpha))
        return false;
    swapchainCreateInfo.compositeAlpha = compositeAlpha;
    return true;
}

bool SwapchainBuilder::setPresentMode(VkPresentModeKHR presentMode) {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode) == surfacePresentModes.end()) {
        return false;
    }
    swapchainCreateInfo.presentMode = presentMode;
    return true;
}

void SwapchainBuilder::setClipped(VkBool32 clipped) {
    swapchainCreateInfo.clipped = clipped;
}

void SwapchainBuilder::setOldSwapchain(VkSwapchainKHR oldSwapchain) {
    swapchainCreateInfo.oldSwapchain = oldSwapchain;
}

bool SwapchainBuilder::setSwapchainCreateInfo(VkSwapchainCreateInfoKHR swapchainCreateInfo) {
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
        if(!setPresentMode(VK_PRESENT_MODE_FIFO_KHR))
            // TODO: should log this
            return false;
    }
    return true;
}

bool SwapchainBuilder::setToDoubleBuffering() {
    if(!setMinImageCount(2))
        // TODO: should log this
        return false;
    return enableVsyncPresentMode();
}

bool SwapchainBuilder::setToTripleBuffering() {
    if(!setMinImageCount(3))
        // TODO: should log this
        return false;
    return enableVsyncPresentMode();
}

void SwapchainBuilder::querySurfaceCapabilities() {
    check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
              physicalDevice, swapchainCreateInfo.surface, &surfaceCapabilities),
          "Failed to query Surface Capabilities on Swapchain Creation!");
}

void SwapchainBuilder::querySurfaceFormats() {
    // TODO: write a "vulkan check result" function that allows to pass an error message and a result
    uint32_t surfaceFormatCount;
    check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                               swapchainCreateInfo.surface,
                                               &surfaceFormatCount, nullptr),
          "Failed to query Surface Formats on Swapchain Creation!");
    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
    check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                               swapchainCreateInfo.surface, &surfaceFormatCount,
                                               surfaceFormats.data()),
          "Failed to query Surface Formats on Swapchain Creation!");
}

void SwapchainBuilder::querySurfacePresentModes() {
    uint32_t presentModeCount;
    check(vkGetPhysicalDeviceSurfacePresentModesKHR(
              physicalDevice, swapchainCreateInfo.surface, &presentModeCount, nullptr),
          "Failed to query Surface Present Modes on Swapchain Creation!");
    std::vector<VkPresentModeKHR> presentModes;
    check(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                    swapchainCreateInfo.surface, &presentModeCount,
                                                    presentModes.data()),
          "Failed to query Surface Present Modes on Swapchain Creation!");
}



// TODO: all these check functions should log if an argument does not pass!


bool SwapchainBuilder::checkMinImageCountSupport(uint32_t minImageCount) {
    if(minImageCount < surfaceCapabilities.minImageCount
       || minImageCount > surfaceCapabilities.maxImageCount) {
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

bool SwapchainBuilder::checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat) {
    if(std::find(surfaceFormats.begin(), surfaceFormats.end(), imageFormat)
       == surfaceFormats.end())
        return false;
    return true;
}

bool SwapchainBuilder::checkImageExtentSupport(VkExtent2D imageExtent) {
    surfaceCapabilities.minImageExtent;
    if(imageExtent.width < surfaceCapabilities.minImageExtent.width
       || imageExtent.width > surfaceCapabilities.maxImageExtent.width
       || imageExtent.height < surfaceCapabilities.minImageExtent.height
       || imageExtent.height > surfaceCapabilities.maxImageExtent.height)
        return false;
    return true;
}

bool SwapchainBuilder::checkImageArrayLayersSupport(uint32_t imageArrayLayers) {
    if(imageArrayLayers > surfaceCapabilities.maxImageArrayLayers)
        return false;
    return true;
}

bool SwapchainBuilder::checkImageUsageSupport(VkImageUsageFlags imageUsage) {
    if((surfaceCapabilities.supportedUsageFlags | imageUsage)
       != surfaceCapabilities.supportedUsageFlags)
        return false;
    return true;
}

bool SwapchainBuilder::checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform) {
    if((surfaceCapabilities.supportedTransforms | preTransform)
       != surfaceCapabilities.supportedTransforms)
        return false;
    return true;
}

bool SwapchainBuilder::checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha) {
    if((surfaceCapabilities.supportedCompositeAlpha | compositeAlpha)
       != surfaceCapabilities.supportedCompositeAlpha)
        return false;
    return true;
}

bool SwapchainBuilder::checkPresentModeSupport(VkPresentModeKHR presentMode) {
    if(std::find(surfacePresentModes.begin(), surfacePresentModes.end(), presentMode)
       == surfacePresentModes.end())
        return false;
    return true;
}
