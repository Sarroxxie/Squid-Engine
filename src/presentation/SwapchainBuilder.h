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
    // TODO: decide on a better ordering of functions here -> build first, then physicalDevice + surface, then rest
    // TODO: implement the function variant that do not take a "bool& success" as parameter

    SwapchainBuilder();
    SwapchainBuilder(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
    // may throw exception
    SwapchainBuilder& setPhysicalDevice(const VkPhysicalDevice& physicalDevice);
    // TODO: documentation! -> needs valid physical device and surface
    void build(const VkDevice& device, VkSwapchainKHR& swapchain) const;

    // TODO should all setters also return a SwapchainBuilder& back? for function
    // chaining? this would match the behavior of the InstanceBuilder and the DeviceBuilder
    // => YES! instead of bool outputs it should throw a runtime error! -> or instead define new exceptions!

    // idea: make it optional to get feedback if things worked -> optional "bool& success" -> two versions of functions

    // checks parameters for support and set fields inside the VkSwapchainCreateInfoKHR
    SwapchainBuilder& setFlags(const VkSwapchainCreateFlagsKHR flags);
    // may throw exception
    SwapchainBuilder& setSurface(const VkSurfaceKHR surface);

    SwapchainBuilder& setMinImageCount(const uint32_t minImageCount, bool& success);
    SwapchainBuilder& setMinImageCount(const uint32_t minImageCount);

    SwapchainBuilder& setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat, bool& success);
    SwapchainBuilder& setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat);

    SwapchainBuilder& setImageExtent(const VkExtent2D imageExtent, bool& success);
    SwapchainBuilder& setImageExtent(const VkExtent2D imageExtent);

    SwapchainBuilder& setImageArrayLayers(const uint32_t imageArrayLayers, bool& success);
    SwapchainBuilder& setImageArrayLayers(const uint32_t imageArrayLayers);

    SwapchainBuilder& setImageUsage(const VkImageUsageFlags imageUsage, bool& success);
    SwapchainBuilder& setImageUsage(const VkImageUsageFlags imageUsage);

    SwapchainBuilder& setImageSharingMode(const VkSharingMode imageSharingMode);
    SwapchainBuilder& setQueueFamilyIndices(const std::vector<uint32_t> queueFamilyIndices);


    SwapchainBuilder& setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform,
                                      bool& success);
    SwapchainBuilder& setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform);


    SwapchainBuilder& setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha, bool& success);
    SwapchainBuilder& setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha);

    SwapchainBuilder& setPresentMode(const VkPresentModeKHR presentMode, bool& success);
    SwapchainBuilder& setPresentMode(const VkPresentModeKHR presentMode);

    SwapchainBuilder& setClipped(const VkBool32 clipped);
    SwapchainBuilder& setOldSwapchain(const VkSwapchainKHR oldSwapchain);

    // may throw exception
    SwapchainBuilder& setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo, bool& success);
    SwapchainBuilder& setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo);

    SwapchainBuilder& enableVsyncPresentMode(bool& success);
    SwapchainBuilder& enableVsyncPresentMode();

    SwapchainBuilder& setToDoubleBuffering(bool& success);
    SwapchainBuilder& setToDoubleBuffering();

    SwapchainBuilder& setToTripleBuffering(bool& success);
    SwapchainBuilder& setToTripleBuffering();

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

    // TODO: checks if the content of the create info is supported
    bool checkCreateInfoSupport() const;
    bool checkMinImageCountSupport(uint32_t minImageCount) const;
    bool checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat) const;
    bool checkImageExtentSupport(VkExtent2D imageExtent) const;
    bool checkImageArrayLayersSupport(uint32_t imageArrayLayers) const;
    bool checkImageUsageSupport(VkImageUsageFlags imageUsage) const;
    bool checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform) const;
    bool checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha) const;
    bool checkPresentModeSupport(VkPresentModeKHR presentMode) const;
};