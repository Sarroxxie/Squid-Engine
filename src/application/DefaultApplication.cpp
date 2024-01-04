#include "DefaultApplication.h"

#include "setup/InstanceBuilder.h"
#include "setup/DefaultPhysicalDeviceSelector.h"
#include "setup/DeviceBuilder.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

DefaultApplication::DefaultApplication(Window* window)
    : Application::Application(window) {
    init();
    // To get a working debugMessenger, we need the call to the non-default
    // constructor which requires a valid instance.
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

void DefaultApplication::createSurface() {}

void DefaultApplication::selectPhysicalDevice() {
    DefaultPhysicalDeviceSelector selector;
    try {
        physicalDevice = selector.selectPhysicalDevice(instance);
    } catch(std::runtime_error& re) {
        std::cerr << "ERROR: " << re.what()
                  << " -> destroying all previously created resources...\n";
        // Clean all resources before exiting the process to prevent undefined behavior.
        cleanup();
        exit(-1);
    }
}

void DefaultApplication::createDevice() {
    DeviceBuilder builder(physicalDevice);
    try {
        builder.build(device);
    } catch(std::runtime_error& re) {
        std::cerr << re.what() << " -> destroying all previously created resources...\n";
        cleanup();
        exit(-1);
    }
}
