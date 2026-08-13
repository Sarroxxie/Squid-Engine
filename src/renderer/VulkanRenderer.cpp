#include "VulkanRenderer.h"

#include "output/Logger.h"

VulkanRenderer::VulkanRenderer(Window* const window)
    : window(window) {}

void VulkanRenderer::init() {
    createInstance();
    createSurface();
    selectPhysicalDevice();
    createDevice();
    createSwapchain();
    createGraphicsPipeline();
}

const Window* VulkanRenderer::getWindow() const {
    return window;
}

void VulkanRenderer::cleanup() {
    if(swapchain.handle != VK_NULL_HANDLE) {
        swapchain.destroy(device);
        SLOG_INFO("Swawpchain cleanup complete.");
    }

    if(device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, nullptr);
        SLOG_INFO("Logical Device cleanup complete.");
    }

    if(surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        SLOG_INFO("Surface cleanup complete.");
    }

    if(instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        SLOG_INFO("Instance cleanup complete.");
    }

    SLOG_INFO("Vulkan cleanup complete.");

    window->cleanup();
}