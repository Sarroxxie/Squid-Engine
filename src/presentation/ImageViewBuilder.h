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

// TODO: as we use more different mapping, create them here instead of in the
// code that calls ImageViewBuilder::build()
namespace ComponentMappings {

constexpr VkComponentMapping RGBA{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                  .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                  .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                  .a = VK_COMPONENT_SWIZZLE_IDENTITY};


constexpr VkComponentMapping IDENTITY = RGBA;

}  // namespace ComponentMappings


constexpr VkImageSubresourceRange DEFAULT_IMAGE_SUBRESOURCE_RANGE{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                  .baseMipLevel = 0,
                                                                  .levelCount = 1,
                                                                  .baseArrayLayer = 0,
                                                                  .layerCount = 1};

constexpr VkImageViewCreateInfo DEFAULT_IMAGE_VIEW_CREATE_INFO{
    .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext            = nullptr,
    .flags            = 0,
    .image            = VK_NULL_HANDLE,
    .viewType         = VK_IMAGE_VIEW_TYPE_2D,
    .format           = VK_FORMAT_B8G8R8A8_SRGB,
    .components       = ComponentMappings::IDENTITY,
    .subresourceRange = DEFAULT_IMAGE_SUBRESOURCE_RANGE};

/*
Wrapper for VkImageView creation.
*/
class ImageViewBuilder
{
  public:
    ImageViewBuilder();
    ImageViewBuilder(const VkImage image);

    // TODO: for the future: check if we need to access the createInfo again
    //       when using the image view
    //       -> could consider to create a "ImageView" class that stores these
    //          and return it instead
    void build(const VkDevice& device, VkImageView& imageView) const;

    // TODO: write documentation!

    ImageViewBuilder& setFlags(const VkImageViewCreateFlags flags);
    ImageViewBuilder& setImage(const VkImage image);
    ImageViewBuilder& setViewType(const VkImageViewType viewType);
    ImageViewBuilder& setFormat(const VkFormat format);
    ImageViewBuilder& setComponents(const VkComponentMapping components);

    ImageViewBuilder& setSubresourceRange(const VkImageSubresourceRange subresourceRange);
    ImageViewBuilder& setAspectMask(const VkImageAspectFlags aspectMask);
    ImageViewBuilder& setBaseMipLevel(const uint32_t baseMipLevel);
    ImageViewBuilder& setLevelCount(const uint32_t levelCount);
    ImageViewBuilder& setBaseArrayLayer(const uint32_t baseArrayLayer);
    ImageViewBuilder& setLayerCount(const uint32_t layerCount);

  private:
    VkImageViewCreateInfo imageViewCreateInfo = DEFAULT_IMAGE_VIEW_CREATE_INFO;
};