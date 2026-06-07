#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/*
 * Wrapper for creation of a VkSwapchainKHR. It is recommended to store this
 * builder to speed up Swapchain recreation (as we can omit some support queries
 * then). Automatically queries the physical device and surface for support of
 * the used parameters.
 */
class SwapchainBuilder
{
  public:
    SwapchainBuilder();
    SwapchainBuilder(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
    SwapchainBuilder& setPhysicalDevice(const VkPhysicalDevice& physicalDevice);
    // TODO: documentation! -> needs valid physical device and surface
    void build(const VkDevice& device, VkSwapchainKHR& swapchain) const;

    // TODO should all setters also return a SwapchainBuilder& back? for function
    // chaining? this would match the behavior of the InstanceBuilder and the DeviceBuilder
    // => YES! instead of bool outputs it should throw a runtime error! -> or instead define new exceptions!

    // checks parameters for support and set fields inside the VkSwapchainCreateInfoKHR
    SwapchainBuilder& setFlags(const VkSwapchainCreateFlagsKHR flags);
    SwapchainBuilder& setSurface(const VkSurfaceKHR surface);
    bool setMinImageCount(const uint32_t minImageCount);
    bool setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat);
    bool setImageExtent(const VkExtent2D imageExtent);
    bool setImageArrayLayers(const uint32_t imageArrayLayers);
    bool setImageUsage(const VkImageUsageFlags imageUsage);
    SwapchainBuilder& setImageSharingMode(const VkSharingMode imageSharingMode);
    SwapchainBuilder& setQueueFamilyIndices(const std::vector<uint32_t> queueFamilyIndices);
    bool setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform);
    bool setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha);
    bool setPresentMode(const VkPresentModeKHR presentMode);
    SwapchainBuilder& setClipped(const VkBool32 clipped);
    SwapchainBuilder& setOldSwapchain(const VkSwapchainKHR oldSwapchain);

    bool setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo);

    bool enableVsyncPresentMode();
    bool setToDoubleBuffering();
    bool setToTripleBuffering();

    // TODO: do we need getters to get the current values?

  private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    // default options that are usually supported (TODO: maybe better to check just in case)
    VkSwapchainCreateInfoKHR swapchainCreateInfo{
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .imageFormat      = VK_FORMAT_B8G8R8A8_SRGB,
        .imageColorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageArrayLayers = 1,
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform     = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = VK_PRESENT_MODE_FIFO_KHR,
        .clipped          = VK_FALSE};

    // TODO: -> need to check support on surface change
    VkSurfaceCapabilitiesKHR        surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR>   surfacePresentModes;

    // TODO: needs documentation!
    void querySurfaceCapabilities();
    void querySurfaceFormats();
    void querySurfacePresentModes();

    bool checkMinImageCountSupport(uint32_t minImageCount) const;
    bool checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat) const;
    bool checkImageExtentSupport(VkExtent2D imageExtent) const;
    bool checkImageArrayLayersSupport(uint32_t imageArrayLayers) const;
    bool checkImageUsageSupport(VkImageUsageFlags imageUsage) const;
    bool checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform) const;
    bool checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha) const;
    bool checkPresentModeSupport(VkPresentModeKHR presentMode) const;
};