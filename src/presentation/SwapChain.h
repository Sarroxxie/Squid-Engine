#pragma once

#include <vulkan/vulkan.h>

// TODO: implement a swap chain builder, check in device suitability for swapchain support
//       -> many of the needed functions should be declared separately in a "SwapChainUtils"
//       namespace so they can be reused to recreate the swapchain
class SwapChain
{
  public:
    uint32_t   getWidth() const;
    uint32_t   getHeight() const;
    VkExtent2D getExtent() const;

  private:
    VkExtent2D     extent;
    VkSwapchainKHR swapChain;
};