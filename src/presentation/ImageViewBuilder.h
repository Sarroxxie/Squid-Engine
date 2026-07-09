#pragma once

#include <vulkan/vulkan.h>

// TODO: as we use more different mapping, create them here instead of in the
//       code that calls ImageViewBuilder::build()
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
    /*
    Automatically sets the Image for which the Image View gets created.
    */
    ImageViewBuilder(const VkImage image);

    // TODO: for the future: check if we need to access the createInfo again
    //       when using the image view
    //       -> could consider creating an "ImageView" class that stores these
    //          and return that class instead of the VkImageView
    void build(const VkDevice& device, VkImageView& imageView) const;

    /*
    Sets the Image View Create Info to be used in the Image View creation.
    */
    ImageViewBuilder& setImageViewCreateInfo(const VkImageViewCreateInfo imageViewCreateInfo);

    /*
    Sets the flags to be used in the Image View creation.
    */
    ImageViewBuilder& setFlags(const VkImageViewCreateFlags flags);
    /*
    Sets the Image for which the Image View gets created.
    */
    ImageViewBuilder& setImage(const VkImage image);
    /*
    Sets the View Type for the Image View.
    */
    ImageViewBuilder& setViewType(const VkImageViewType viewType);
    /*
    Sets the Format for the Image View.
    */
    ImageViewBuilder& setFormat(const VkFormat format);
    /*
    Sets the Component Mapping for the Image View.
    */
    ImageViewBuilder& setComponents(const VkComponentMapping components);

    /*
    Sets the Subresource Range for the Image View. This contains the Aspect
    Mask, the Base Mip Level, the Mip Level Count, the Base Array Layer and the
    Base Array Layer Count.
    */
    ImageViewBuilder& setSubresourceRange(const VkImageSubresourceRange subresourceRange);
    /*
    Sets the Aspect Mask for the Image View.
    */
    ImageViewBuilder& setAspectMask(const VkImageAspectFlags aspectMask);
    /*
    Sets the Base Mip Level for the Image View.
    */
    ImageViewBuilder& setBaseMipLevel(const uint32_t baseMipLevel);
    /*
    Sets the Mip Level Count for the Image View.
    */
    ImageViewBuilder& setLevelCount(const uint32_t levelCount);
    /*
    Sets the Base Array Layer for the Image View.
    */
    ImageViewBuilder& setBaseArrayLayer(const uint32_t baseArrayLayer);
    /*
    Sets the Array Layer Count for the Image View.
    */
    ImageViewBuilder& setLayerCount(const uint32_t layerCount);

  private:
    VkImageViewCreateInfo imageViewCreateInfo = DEFAULT_IMAGE_VIEW_CREATE_INFO;
};