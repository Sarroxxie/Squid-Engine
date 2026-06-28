#include "VulkanRenderer.h"

#include "output/Logger.h"

VulkanRenderer::VulkanRenderer(Window* const window) : window(window) {}

void VulkanRenderer::init() {
    createInstance();
    createSurface();
    selectPhysicalDevice();
    createDevice();
    createSwapchain();
}

const Window* VulkanRenderer::getWindow() const {
    return window;
}

void VulkanRenderer::cleanup() {
    if(swapchain.handle != VK_NULL_HANDLE) {
        swapchain.destroy(device);
        SLOG_INFO("Cleaned up Swapchain.");
    }

    if(device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, nullptr);
        SLOG_INFO("Cleaned up Logical Device.");
    }

    if(surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        SLOG_INFO("Cleaned up Surface.");
    }

    if(instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        SLOG_INFO("Cleaned up Instance.");
    }

    SLOG_INFO("Vulkan cleanup complete.");

    window->cleanup();
}