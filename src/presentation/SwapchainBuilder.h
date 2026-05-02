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
    SwapchainBuilder(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    void setPhysicalDevice(VkPhysicalDevice physicalDevice);
    // TODO: documentation! -> needs valid physical device and surface
    void build(VkDevice device, VkSwapchainKHR* swapchain);

    // checks parameters for support and set fields inside the VkSwapchainCreateInfoKHR
    void setFlags(VkSwapchainCreateFlagsKHR flags);
    void setSurface(VkSurfaceKHR surface);
    bool setMinImageCount(uint32_t minImageCount);
    bool setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);
    bool setImageExtent(VkExtent2D imageExtent);
    bool setImageArrayLayers(uint32_t imageArrayLayers);
    bool setImageUsage(VkImageUsageFlags imageUsage);
    void setImageSharingMode(VkSharingMode imageSharingMode);
    void setQueueFamilyIndices(std::vector<uint32_t> queueFamilyIndices);
    bool setPreTransform(VkSurfaceTransformFlagBitsKHR preTransform);
    bool setCompositeAlpha(VkCompositeAlphaFlagBitsKHR compositeAlpha);
    bool setPresentMode(VkPresentModeKHR presentMode);
    void setClipped(VkBool32 clipped);
    void setOldSwapchain(VkSwapchainKHR oldSwapchain);

    bool setSwapchainCreateInfo(VkSwapchainCreateInfoKHR swapchainCreateInfo);

    bool enableVsyncPresentMode();
    bool setToDoubleBuffering();
    bool setToTripleBuffering();

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

    bool checkMinImageCountSupport(uint32_t minImageCount);
    bool checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat);
    bool checkImageExtentSupport(VkExtent2D imageExtent);
    bool checkImageArrayLayersSupport(uint32_t imageArrayLayers);
    bool checkImageUsageSupport(VkImageUsageFlags imageUsage);
    bool checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform);
    bool checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha);
    bool checkPresentModeSupport(VkPresentModeKHR presentMode);
};