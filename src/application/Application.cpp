#include "Application.h"

void Application::init() {
    createInstance();
    selectPhysicalDevice();
}

VkInstance& Application::getInstance() {
    return instance;
}

void Application::cleanup() {
    if(validInstance)
        vkDestroyInstance(instance, nullptr);
}
