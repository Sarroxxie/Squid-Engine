#include "DefaultApplication.h"
#include "setup/InstanceBuilder.h"
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

DefaultApplication::DefaultApplication() {
    initInstance();
}

void DefaultApplication::cleanup() {
    if(USE_DEBUG_UTILS)
        debugMessenger.cleanup(instance);
    Application::cleanup();
}

void DefaultApplication::initInstance() {
    InstanceBuilder builder;
    builder.setAppName("Thesis Renderer");

    std::vector<const char*> layers;
    std::vector<const char*> extensions = getRequiredExtensions();

    if(USE_DEBUG_UTILS) {
        // required when using validation layers
        layers.push_back("VK_LAYER_KHRONOS_validation");
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        // get debug information on instance creation and destruction
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
        DebugUtilsMessenger::populateDebugUtilsMessengerCreateInfo(debugUtilsMessengerCreateInfo);
        builder.attachDebugMessenger(debugUtilsMessengerCreateInfo);
    }

    // As some of the requested extensions or layers could not be supported by
    // the GPU, we need to catch exceptions.
    try {
        builder.requestLayers(layers);
        builder.requestExtensions(extensions);
    } catch(std::runtime_error& re) {
        std::cerr << re.what() << "\n";
        exit(-1);
    }

    // this will internally call "vkCreateInstance(..)"
    builder.build(instance);

    if(USE_DEBUG_UTILS)
        debugMessenger = DebugUtilsMessenger(instance);

    // if instance creation failed, this line is never be called because of the exception
    validInstance = true;
}

std::vector<const char*> DefaultApplication::getRequiredExtensions() {
    std::vector<const char*> extensions;

    // extensions that are required by GLFW
    uint32_t     glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}
