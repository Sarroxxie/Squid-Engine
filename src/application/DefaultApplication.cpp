#include "DefaultApplication.h"
#include "setup/InstanceBuilder.h"
#include "setup/DefaultPhysicalDeviceSelector.h"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

DefaultApplication::DefaultApplication()
    : debugMessenger(DebugUtilsMessenger()) {
    init();
    // As a member variable always has to be initialized, the default constructor is
    // used on the DebugUtilsMessenger. To get a working debugMessenger however,
    // you need the call to the non-default constructor which requires a valid instance.
    if(USE_DEBUG_UTILS)
        debugMessenger = DebugUtilsMessenger(instance);
}

void DefaultApplication::cleanup() {
    if(USE_DEBUG_UTILS)
        debugMessenger.cleanup(instance);
    Application::cleanup();
}

void DefaultApplication::createInstance() {
    InstanceBuilder builder;
    builder.setAppName("Thesis Renderer");

    std::vector<const char*> layers;
    std::vector<const char*> extensions = getRequiredExtensions();

    if(USE_DEBUG_UTILS) {
        // required when using validation layers
        layers.push_back("VK_LAYER_KHRONOS_validation");
        extensions.push_back("VK_EXT_debug_utils");

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
        std::cerr << "ERROR: " << re.what() << "\n";
        exit(-1);
    }

    // this will internally call "vkCreateInstance(..)"
    builder.build(instance);

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

void DefaultApplication::selectPhysicalDevice() {
    DefaultPhysicalDeviceSelector selector;
    try {
        physicalDevice = selector.selectPhysicalDevice(instance);
    } catch(std::runtime_error& re) {
        std::cerr << "ERROR: " << re.what() << " -> destroying all created resources...\n";
        // Clean all resources before exiting the process to prevent undefined behavior.
        cleanup();
        exit(-1);
    }
}
