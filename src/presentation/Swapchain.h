#pragma once

#include <vulkan/vulkan.h>
#include <vector>

struct Swapchain
{
  public:
    /*
    Destroys the Swapchain via Vulkan API call.
    */
    void                 destroy(VkDevice device);

    VkSwapchainKHR           handle = VK_NULL_HANDLE;
    std::vector<VkImage>     images{};
    std::vector<VkImageView> imageViews;
    VkFormat                 imageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D               extent{0, 0};
};