#include "DefaultApplication.h"
#include "setup/InstanceBuilder.h"
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

DefaultApplication::DefaultApplication() {
    init();
}

void DefaultApplication::init() {
    InstanceBuilder builder;
    builder.setAppName("Thesis Renderer");

    std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    std::vector<const char*> extensions = getRequiredExtensions();
    // required for printing validation layer information
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    // As some of the requested extensions or validation layers could not be
    // supported by the GPU, we need to catch exceptions.
    try {
        builder.requestLayers(validationLayers);
        builder.requestExtensions(extensions);
    } catch(std::runtime_error& re) {
        std::cout << re.what() << "\n";
        exit(-1);
    }
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
