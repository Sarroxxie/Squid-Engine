#include "Application.h"

Application::Application(Window* const window)
    : window(window) {}

void Application::init() {
    createInstance();
    createSurface();
    selectPhysicalDevice();
    createDevice();
}

VkInstance& Application::getInstance() {
    return instance;
}

void Application::cleanup() {
    if(device != VK_NULL_HANDLE)
        vkDestroyDevice(device, nullptr);
    if(surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(instance, surface, nullptr);
    if(instance != VK_NULL_HANDLE)
        vkDestroyInstance(instance, nullptr);
}
