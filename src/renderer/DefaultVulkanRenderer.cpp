#include "DefaultVulkanRenderer.h"

#include "setup/InstanceBuilder.h"
#include "setup/DefaultPhysicalDeviceSelector.h"
#include "setup/DeviceBuilder.h"
#include "shader/ShaderCompiler.h"
#include "shader/ShaderModule.h"
#include "output/VulkanCheck.h"
#include "output/Logger.h"
#include "Exceptions.h"

DefaultVulkanRenderer::DefaultVulkanRenderer(Window* const window)
    : DefaultVulkanRenderer(window, false) {}

DefaultVulkanRenderer::DefaultVulkanRenderer(Window* const window, bool useDebugUtils)
    : VulkanRenderer::VulkanRenderer(window)
    , USE_DEBUG_UTILS(useDebugUtils) {
    try {
        init();
        // To get a working debugMessenger, we need the call to the non-default
        // constructor which requires a valid instance.
        // TODO: create this as soon as possible (after Instance creation)!
        //       -> might need to modify the "init()" structure or add VLs to the "VulkanRenderer"
        if(USE_DEBUG_UTILS)
            debugMessenger = DebugUtilsMessenger(instance);
    } catch(std::exception& e) {
        SLOG_FATAL(e.what());
        // destroying all previously created resources
        cleanup();
        exit(EXIT_FAILURE);
    }
}

void DefaultVulkanRenderer::cleanup() {
    if(USE_DEBUG_UTILS)
        debugMessenger.cleanup(instance);
    VulkanRenderer::cleanup();
}

void DefaultVulkanRenderer::createInstance() {
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

std::vector<const char*> DefaultVulkanRenderer::getRequiredExtensions() {
    std::vector<const char*> extensions;

    // extensions that are required by GLFW
    uint32_t     glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

void DefaultVulkanRenderer::createSurface() {
    check<SurfaceCreationException>(
        glfwCreateWindowSurface(instance, window->getWindowHandle(), nullptr, &surface),
        "Window Surface creation failed.");
    SLOG_INFO("Successfully created Surface.");
}

void DefaultVulkanRenderer::selectPhysicalDevice() {
    DefaultPhysicalDeviceSelector selector;
    physicalDevice = selector.selectPhysicalDevice(instance, surface);
}

void DefaultVulkanRenderer::createDevice() {
    DeviceBuilder builder(physicalDevice, surface);
    // required for swap chain
    const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                 VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME};
    QueueFamilyIndices indices;
    builder.requestExtensions(extensions);
    // TODO: may want to specify features here that we will be using

    indices = builder.build(device);
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void DefaultVulkanRenderer::createSwapchain() {
    swapchainBuilder.setPhysicalDevice(physicalDevice);
    swapchainBuilder.setSurface(surface);
    swapchainBuilder.setToTripleBuffering();

    swapchainBuilder.build(device, swapchain);
    // we need this to be able to later access the Image Views of the Swapchain
    swapchain.retrieveSwapchainImages(device);
}

void DefaultVulkanRenderer::createGraphicsPipeline() {
    // TODO: create a single file that contains all necessary functions with a
    //       namespace, so the hardcoded stuff happens elsewhere
    //       -> ShaderStages, Pipeline Setup, Render Pass?
    ShaderModule shaderModule = ShaderModule(
        std::string("rainbow_triangle.slang"),
        std::vector<ShaderEntryPoint>{{"vertMain", VK_SHADER_STAGE_VERTEX_BIT},
                                      {"fragMain", VK_SHADER_STAGE_FRAGMENT_BIT}});

    CommandLineShaderCompiler shaderCompiler;
    shaderModule.compile(&shaderCompiler);

    shaderModule.buildShaderStages(device);


    // shader module can only to be destroyed AFTER the pipeline creation is done
    // TODO: is there any point where we might have to reuse a shader module?
    //       -> if so, we could also add them to the class and cleanup in the end
    shaderModule.destroy(device);

    // TODO: create the remaining graphics pipeline!
}