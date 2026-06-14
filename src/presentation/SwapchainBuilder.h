#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/*
Wrapper for creation of a VkSwapchainKHR. Choose desired settings for the
Swapchain creation first and finally create it using "build". Most functions
have two variants that work exactly the same internally with the only
difference being that you can pass an additional bool to check if the
operation was successful. It is recommended to store this builder to speed up
Swapchain recreation (as we can omit some support queries then).
As most setters check if the requested parameter is supported and only then
set the corresponding value, it is recommended to first set a valid Physical
Device and Surface.
 */
class SwapchainBuilder
{
  public:
    /*
    Dummy constructor so this class can be initialized before a Physical Device
    or Surface exists.
    */
    SwapchainBuilder();
    /*
    Already set the Physical Device and Surface on object creation.
    */
    SwapchainBuilder(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

    /*
    Creates a Swapchain from the parameters that were specified before calling
    this function. As this function requires a valid Physical Device and
    Surface, it should only be called after setting those.
    */
    void build(const VkDevice& device, VkSwapchainKHR& swapchain) const;

    /*
    Sets the Physical Device to be used in the Swapchain creation. If a valid
    Surface is already set, the requested settings are checked for support and
    if necessary, they are reset to the default values.
    Returns true if setting the Physical Device was successful.
    */
    SwapchainBuilder& setPhysicalDevice(const VkPhysicalDevice& physicalDevice, bool& success);
    /*
    Sets the Physical Device to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setPhysicalDevice(const VkPhysicalDevice& physicalDevice);

    /*
    Sets the Surface to be used in the Swapchain creation. If a valid
    Physical Device is already set, the requested settings are checked for support and
    if necessary, they are reset to the default values.
    Returns true if setting the Surface was successful.
    */
    SwapchainBuilder& setSurface(const VkSurfaceKHR surface, bool& success);
    /*
    Sets the Surface to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setSurface(const VkSurfaceKHR surface);

    /*
    Sets the Surface to be used in the Swapchain creation as well as the
    remaining settings from the passed SwapchainCreateInfo. The settings are
    immediately checked for support if a valid Physical Device exists and if
    not, they are checked when the Physical Device is set.
    Returns true if setting the Surface was successful. (if any other setting is
    not supported but the surface was valid, then this function still returns true!)
    */
    SwapchainBuilder& setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo,
                                             bool& success);
    /*
    Sets the Surface to be used in the Swapchain creation as well as the
    remaining settings from the passed SwapchainCreateInfo. The settings are
    immediately checked for support if a valid Physical Device exists and if
    not, they are checked when the Physical Device is set.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setSwapchainCreateInfo(const VkSwapchainCreateInfoKHR swapchainCreateInfo);

    /*
    Sets the flags to be used in the Swapchain creation.
    */
    SwapchainBuilder& setFlags(const VkSwapchainCreateFlagsKHR flags);

    /*
    Sets the minImageCount to be used in the Swapchain creation if it is
    supported. Returns true if setting the minImageCount was successful.
    */
    SwapchainBuilder& setMinImageCount(const uint32_t minImageCount, bool& success);
    /*
    Sets the minImageCount to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setMinImageCount(const uint32_t minImageCount);

    /*
    Sets the surfaceFormat to be used in the Swapchain creation if it is
    supported. Returns true if setting the surfaceFormat was successful.
    */
    SwapchainBuilder& setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat, bool& success);
    /*
    Sets the surfaceFormat to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setSurfaceFormat(const VkSurfaceFormatKHR surfaceFormat);

    /*
    Sets the imageExtent to be used in the Swapchain creation if it is
    supported. Returns true if setting the imageExtent was successful.
    */
    SwapchainBuilder& setImageExtent(const VkExtent2D imageExtent, bool& success);
    /*
    Sets the imageExtent to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setImageExtent(const VkExtent2D imageExtent);

    /*
    Sets the imageArrayLayers value to be used in the Swapchain creation if it
    is supported. Returns true if setting the imageArrayLayers was successful.
    */
    SwapchainBuilder& setImageArrayLayers(const uint32_t imageArrayLayers, bool& success);
    /*
    Sets the imageArrayLayers value to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setImageArrayLayers(const uint32_t imageArrayLayers);

    /*
    Sets the imageUsage flags to be used in the Swapchain creation if it is
    supported. Returns true if setting the imageUsage was successful.
    */
    SwapchainBuilder& setImageUsage(const VkImageUsageFlags imageUsage, bool& success);
    /*
    Sets the imageUsage flags to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setImageUsage(const VkImageUsageFlags imageUsage);

    /*
    Sets the imageSharingMode to be used in the Swapchain creation.
    */
    SwapchainBuilder& setImageSharingMode(const VkSharingMode imageSharingMode);

    /*
    Sets the queueFamilyIndices to be used in the Swapchain creation. Only
    needed if the imageSharingMode is set to VK_SHARING_MODE_CONCURRENT.
    */
    SwapchainBuilder& setQueueFamilyIndices(const std::vector<uint32_t> queueFamilyIndices);

    /*
    Sets the preTransform to be used in the Swapchain creation if it is
    supported. Returns true if setting the preTransform was successful.
    */
    SwapchainBuilder& setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform,
                                      bool& success);
    /*
    Sets the preTransform to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setPreTransform(const VkSurfaceTransformFlagBitsKHR preTransform);

    /*
    Sets the compositeAlpha value to be used in the Swapchain creation if it is
    supported. Returns true if setting the compositeAlpha was successful.
    */
    SwapchainBuilder& setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha,
                                        bool& success);
    /*
    Sets the compositeAlpha value to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setCompositeAlpha(const VkCompositeAlphaFlagBitsKHR compositeAlpha);

    /*
    Sets the presentMode to be used in the Swapchain creation if it is
    supported. Returns true if setting the presentMode was successful.
    */
    SwapchainBuilder& setPresentMode(const VkPresentModeKHR presentMode, bool& success);
    /*
    Sets the presentMode to be used in the Swapchain creation.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setPresentMode(const VkPresentModeKHR presentMode);

    /*
    Sets the clipped value to be used in the Swapchain creation.
    */
    SwapchainBuilder& setClipped(const VkBool32 clipped);

    /*
    Sets the oldSwapchain value to be used in the Swapchain creation.
    */
    SwapchainBuilder& setOldSwapchain(const VkSwapchainKHR oldSwapchain);

    /*
    Sets the presentMode to a mode that supports Vsync. Returns true if setting
    the presentMode was successful.
    */
    SwapchainBuilder& enableVsyncPresentMode(bool& success);
    /*
    Sets the presentMode to a mode that supports Vsync.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& enableVsyncPresentMode();

    /*
    Sets the presentMode to a mode that supports Vsync and minImages to 2.
    Returns true if setting the presentMode and minImageCount was successful.
    */
    SwapchainBuilder& setToDoubleBuffering(bool& success);
    /*
    Sets the presentMode to a mode that supports Vsync and minImages to 2.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setToDoubleBuffering();

    /*
    Sets the presentMode to a mode that supports Vsync and minImages to 3.
    Returns true if setting the presentMode and minImageCount was successful.
    */
    SwapchainBuilder& setToTripleBuffering(bool& success);
    /*
    Sets the presentMode to a mode that supports Vsync and minImages to 3.
    Works exactly like the variant of the function that can return whether the
    operation was successful.
    */
    SwapchainBuilder& setToTripleBuffering();

    /*
    Resets all settings to commonly supported default values.
    */
    SwapchainBuilder& setToDefaultSettings();

  private:
    VkPhysicalDevice         physicalDevice      = VK_NULL_HANDLE;
    VkSwapchainCreateInfoKHR swapchainCreateInfo = defaultSwapchainCreateInfo;

    static constexpr VkSwapchainCreateInfoKHR defaultSwapchainCreateInfo{
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .flags                 = 0,
        .minImageCount         = 2,
        .imageFormat           = VK_FORMAT_B8G8R8A8_SRGB,
        .imageColorSpace       = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent           = VkExtent2D{0, 0},
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = nullptr,
        .preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = VK_PRESENT_MODE_FIFO_KHR,
        .clipped               = VK_FALSE,
        .oldSwapchain          = VK_NULL_HANDLE};


    // need to buffer this to avoid that they go out of scope before "build" has been called
    std::vector<uint32_t> queueFamilyIndices;

    // these only get calculated once on surface change and then stored for
    // performance reasons invariant: these contain valid values once a valid
    // physical device and surface are set
    VkSurfaceCapabilitiesKHR        surfaceCapabilities{.minImageCount = 0,
                                                        .maxImageCount = 0,
                                                        .currentExtent = VkExtent2D{0, 0},
                                                        .minImageExtent = VkExtent2D{0, 0},
                                                        .maxImageExtent = VkExtent2D{0, 0},
                                                        .maxImageArrayLayers = 0,
                                                        .supportedTransforms = 0,
                                                        .currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                                        .supportedCompositeAlpha = 0,
                                                        .supportedUsageFlags = 0};
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR>   surfacePresentModes;

    /*
    Queries surface capabilities. Requires a valid Physical Device and Surface.
    */
    void querySurfaceCapabilities();
    /*
    Queries supported surface formats. Requires a valid Physical Device and Surface.
    */
    void querySurfaceFormats();
    /*
    Queries supported surface present modes. Requires a valid Physical Device and Surface.
    */
    void querySurfacePresentModes();

    /*
    Checks if all the settings of the current SurfaceCreateInfo are supported by the Surface.
    */
    bool checkCreateInfoSupport() const;
    /*
    Checks if the requested minImageCount is supported by the Surface.
    */
    bool checkMinImageCountSupport(uint32_t minImageCount) const;
    /*
    Checks if the requested imageFormat is supported by the Surface.
    */
    bool checkSurfaceFormatSupport(VkSurfaceFormatKHR imageFormat) const;
    /*
    Checks if the requested imageExtent is supported by the Surface.
    */
    bool checkImageExtentSupport(VkExtent2D imageExtent) const;
    /*
    Checks if the requested imageArrayLayers value is supported by the Surface.
    */
    bool checkImageArrayLayersSupport(uint32_t imageArrayLayers) const;
    /*
    Checks if the requested imageUsage flags are supported by the Surface.
    */
    bool checkImageUsageSupport(VkImageUsageFlags imageUsage) const;
    /*
    Checks if the requested preTransform is supported by the Surface.
    */
    bool checkPreTransformSupport(VkSurfaceTransformFlagBitsKHR preTransform) const;
    /*
    Checks if the requested compositeAlpha value is supported by the Surface.
    */
    bool checkCompositeAlphaSupport(VkCompositeAlphaFlagBitsKHR compositeAlpha) const;
    /*
    Checks if the requested presentMode is supported by the Surface.
    */
    bool checkPresentModeSupport(VkPresentModeKHR presentMode) const;
};