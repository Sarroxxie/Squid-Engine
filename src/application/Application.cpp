#include "Application.h"

#include "output/Logger.h"

Application::Application(Window* const window)
    : window(window) {}

void Application::init() {
    createInstance();
    createSurface();
    selectPhysicalDevice();
    createDevice();
    createSwapchain();
}

VkInstance& Application::getInstance() {
    return instance;
}

void Application::cleanup() {
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

    SLOG_INFO("Application cleanup complete.");
}
