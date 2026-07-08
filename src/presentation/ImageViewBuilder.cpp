#include "ImageViewBuilder.h"

#include "output/VulkanCheck.h"
#include "output/Logger.h"
#include "Exceptions.h"

ImageViewBuilder::ImageViewBuilder() {}

ImageViewBuilder::ImageViewBuilder(const VkImage image) {
    imageViewCreateInfo.image = image;
}

void ImageViewBuilder::build(const VkDevice& device, VkImageView& imageView) const {
    check<ImageViewCreationException>(device != VK_NULL_HANDLE, "Device is invalid.");

    check<ImageViewCreationException>(imageViewCreateInfo.image != VK_NULL_HANDLE,
                                      "Image is invalid.");

    check<ImageViewCreationException>(vkCreateImageView(device, &imageViewCreateInfo,
                                                        nullptr, &imageView),
                                      "Failed to create Image View.");

    SLOG_INFO("Successfully created Image View.");
}

ImageViewBuilder& ImageViewBuilder::setFlags(const VkImageViewCreateFlags flags) {
    imageViewCreateInfo.flags = flags;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setImage(const VkImage image) {
    imageViewCreateInfo.image = image;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setViewType(const VkImageViewType viewType) {
    imageViewCreateInfo.viewType = viewType;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setFormat(const VkFormat format) {
    imageViewCreateInfo.format = format;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setComponents(const VkComponentMapping components) {
    imageViewCreateInfo.components = components;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setSubresourceRange(const VkImageSubresourceRange subresourceRange) {
    imageViewCreateInfo.subresourceRange = subresourceRange;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setAspectMask(const VkImageAspectFlags aspectMask) {
    imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setBaseMipLevel(const uint32_t baseMipLevel) {
    imageViewCreateInfo.subresourceRange.baseMipLevel = baseMipLevel;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setLevelCount(const uint32_t levelCount) {
    imageViewCreateInfo.subresourceRange.levelCount = levelCount;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setBaseArrayLayer(const uint32_t baseArrayLayer) {
    imageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
    return *this;
}

ImageViewBuilder& ImageViewBuilder::setLayerCount(const uint32_t layerCount) {
    imageViewCreateInfo.subresourceRange.layerCount = layerCount;
    return *this;
}