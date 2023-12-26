#include "Application.h"

VkInstance& Application::getInstance() {
    return instance;
}

void Application::cleanup() {
    if(validInstance)
        vkDestroyInstance(instance, nullptr);
}
