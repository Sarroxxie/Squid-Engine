#include "Swapchain.h"

#include "output/Logger.h"
#include "output/VulkanCheck.h"
#include "Exceptions.h"
#include "ImageViewBuilder.h"

void Swapchain::destroy(VkDevice device) {
    if(handle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, handle, nullptr);
        SLOG_INFO("Destroyed Swapchain.");

        for(size_t i = 0; i < imageViews.size(); i++) {
            vkDestroyImageView(device, imageViews[i], nullptr);
            SLOG_INFO("Destroyed Swapchain Image View " << i << ".");
        }
    }
}

void Swapchain::retrieveSwapchainImages(VkDevice device) {
    check<SwapchainImageRetrievalException>(device != VK_NULL_HANDLE, "Device is invalid.");
    check<SwapchainImageRetrievalException>(handle != VK_NULL_HANDLE, "Swapchain is invalid.");

    // extracting swapchain images
    uint32_t swapchainImageCount;

    check<SwapchainCreationException>(
        vkGetSwapchainImagesKHR(device, handle, &swapchainImageCount, nullptr),
        "Failed to retrieve Swapchain Images.");
    images.resize(swapchainImageCount);
    check<SwapchainCreationException>(
        vkGetSwapchainImagesKHR(device, handle, &swapchainImageCount, images.data()),
        "Failed to retrieve Swapchain Images.");

    // create ImageViews for the Swapchain Images
    imageViews.resize(images.size());

    ImageViewBuilder imageViewBuilder;
    imageViewBuilder.setFormat(imageFormat);

    for(size_t i = 0; i < images.size(); i++) {
        imageViewBuilder.setImage(images[i]);

        SLOG_DEBUG("Attempting to create Image View for Swapchain Image " << i << ".");
        imageViewBuilder.build(device, imageViews[i]);
    }
    SLOG_INFO("Successfully retrieved Swapchain Images.");
}