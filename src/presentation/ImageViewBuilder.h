#pragma once

#include <vulkan/vulkan.h>

/*
TODO: the basic idea here is to support image view building like the other
builders (with a builder class) but also to support static functions for default
types -> ImageViewBuilder::createImageView(VkDevice device, VkImage image, ...)
      to cover the most common cases (and also "createFromCreateInfo")

      -> the functions in this file should handle errors (image is empty, device
is empty, createImageView failing...)
      -> use class for default builder case
      -> use namespace for static creation
*/

constexpr VkComponentMapping defaultComponentMapping{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     .a = VK_COMPONENT_SWIZZLE_IDENTITY};

constexpr VkImageSubresourceRange defaultImageSubresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                               .baseMipLevel = 0,
                                                               .levelCount = 1,
                                                               .baseArrayLayer = 0,
                                                               .layerCount = 1};

constexpr VkImageViewCreateInfo defaultImageViewCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext            = nullptr,
    .flags            = 0,
    .image            = VK_NULL_HANDLE,
    .viewType         = VK_IMAGE_VIEW_TYPE_2D,
    .format           = VK_FORMAT_B8G8R8A8_SRGB,
    .components       = defaultComponentMapping,
    .subresourceRange = defaultImageSubresourceRange};

class ImageViewBuilder
{


  private:
    VkImageViewCreateInfo imageViewCreateInfo = defaultImageViewCreateInfo;
};