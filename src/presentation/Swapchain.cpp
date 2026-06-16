#include "Swapchain.h"

#include "output/Logger.h"

void Swapchain::destroy(VkDevice device) {
    if(handle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, handle, nullptr);
        SLOG_INFO("Cleaned up Swapchain.");
    }
}