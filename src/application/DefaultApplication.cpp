#include "DefaultApplication.h"

#include "setup/InstanceBuilder.h"
#include "setup/DefaultPhysicalDeviceSelector.h"
#include "setup/DeviceBuilder.h"
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
    // the GPU or instance creation might fail, we need to catch exceptions.
    try {
        builder.requestLayers(layers);
        builder.requestExtensions(extensions);
        // this will internally call "vkCreateInstance(..)"
        builder.build(instance);
    } catch(std::runtime_error& re) {
        std::cerr << "ERROR: " << re.what() << "\n";
        exit(-1);
    }
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

void DefaultApplication::createSurface() {
    if(glfwCreateWindowSurface(instance, window->getWindowHandle(), nullptr, &surface)
       != VK_SUCCESS) {
        std::cerr << "ERROR: Window Surface creation failed !->destroying all previously created resources...\n ";
        // Clean all resources before exiting the process to prevent undefined behavior.
        cleanup();
        exit(-1);
    }
}

void DefaultApplication::selectPhysicalDevice() {
    DefaultPhysicalDeviceSelector selector;
    try {
        physicalDevice = selector.selectPhysicalDevice(instance, surface);
    } catch(std::runtime_error& re) {
        std::cerr << "ERROR: " << re.what()
                  << " -> destroying all previously created resources...\n";
        // Clean all resources before exiting the process to prevent undefined behavior.
        cleanup();
        exit(-1);
    }
}

void DefaultApplication::createDevice() {
    DeviceBuilder builder(physicalDevice, surface);
    // required for swap chain
    const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    QueueFamilyIndices indices;
    try {
        builder.requestExtensions(extensions);
        indices = builder.build(device);
    } catch(std::runtime_error& re) {
        std::cerr << "ERROR: " << re.what()
                  << " -> destroying all previously created resources...\n";
        // Clean all resources before exiting the process to prevent undefined behavior.
        cleanup();
        exit(-1);
    }
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}
