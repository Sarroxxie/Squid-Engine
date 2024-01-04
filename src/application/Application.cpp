#include "Application.h"

void Application::init() {
    createInstance();
    validInstance = true;
    selectPhysicalDevice();
    createDevice();
    validDevice = true;
}

VkInstance& Application::getInstance() {
    return instance;
}

void Application::cleanup() {
    if(validDevice)
        vkDestroyDevice(device, nullptr);
    if(validInstance)
        vkDestroyInstance(instance, nullptr);
}
