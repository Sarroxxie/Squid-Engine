#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/*
Wrapper for the VkSwapchainKHR.
Usage: Create a valid Swapchain handle externally. Then call "retrieveSwapchainImages()"
to create Image Views for the Images of the Swapchain.
*/
struct Swapchain
{
  public:
    /*
    Destroys the Swapchain via Vulkan API call.
    */
    void destroy(const VkDevice& device);

    /*
    Retrieves the Images of the Swapchain and creates Image Views for them.
    Will throw an exception if either the Device or the Swapchain handle is invalid.
    */
    void retrieveSwapchainImages(const VkDevice& device);

    VkSwapchainKHR           handle = VK_NULL_HANDLE;
    std::vector<VkImage>     images{};
    std::vector<VkImageView> imageViews;
    VkFormat                 imageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D               imageExtent{0, 0};
};