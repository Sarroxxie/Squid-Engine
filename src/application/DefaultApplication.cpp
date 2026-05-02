#include "DefaultApplication.h"

#include "setup/InstanceBuilder.h"
#include "setup/DefaultPhysicalDeviceSelector.h"
#include "setup/DeviceBuilder.h"
#include "output/VulkanCheck.h"
#include "output/Logger.h"

DefaultApplication::DefaultApplication(Window* window) : DefaultApplication(window, false) {}

DefaultApplication::DefaultApplication(Window* window, bool useDebugUtils)
    : Application::Application(window)
    , USE_DEBUG_UTILS(useDebugUtils) {
    try {
        init();
        // To get a working debugMessenger, we need the call to the non-default
        // constructor which requires a valid instance.
        if(USE_DEBUG_UTILS)
            debugMessenger = DebugUtilsMessenger(instance);
    } catch(std::runtime_error& e) {
        SLOG_FATAL(e.what());
        // destroying all previously created resources
        cleanup();
        exit(EXIT_FAILURE);
    }
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
    builder.requestLayers(layers);
    builder.requestExtensions(extensions);
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

void DefaultApplication::createSurface() {
    check(glfwCreateWindowSurface(instance, window->getWindowHandle(), nullptr, &surface), "Window Surface creation failed.");
}

void DefaultApplication::selectPhysicalDevice() {
    DefaultPhysicalDeviceSelector selector;
    physicalDevice = selector.selectPhysicalDevice(instance, surface);
}

void DefaultApplication::createDevice() {
    DeviceBuilder builder(physicalDevice, surface);
    // required for swap chain
    const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    QueueFamilyIndices indices;
    builder.requestExtensions(extensions);
    indices = builder.build(device);
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void DefaultApplication::createSwapchain() {
    swapchainBuilder.setPhysicalDevice(this->physicalDevice);
    swapchainBuilder.setSurface(this->surface);
    //swapchainBuilder.setToTripleBuffering(); // as this returns a bool, can check if it worked (TODO?)

    swapchainBuilder.build(this->device, this->swapchain);
}