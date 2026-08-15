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

    // TODO: create the remaining graphics pipeline!
    //       -> need GraphicsPipelineBuilder + store configs for recreation!
    //          (Wrapper for GraphicsPipeline?)

    // TODO: transfer graphics pipeline creation to own class, it's just created
    //       here to get a better overview

    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                                 VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates    = dynamicStates.data()};

    // will be hard coded inside shader
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount   = 0,
        .pVertexBindingDescriptions      = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions    = nullptr};

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{
        .sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext    = nullptr,
        .flags    = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE};

    VkViewport viewport{.x        = 0.0f,
                        .y        = 0.0f,
                        .width    = (float)swapchain.imageExtent.width,
                        .height   = (float)swapchain.imageExtent.height,
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f};

    VkRect2D scissor{.offset = {0, 0}, .extent = swapchain.imageExtent};

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo{
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = 0,
        .viewportCount = 1,
        .pViewports    = &viewport,
        .scissorCount  = 1,
        .pScissors     = &scissor};

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable        = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode             = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,  // TODO: enable backface culling here later
        .frontFace               = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable         = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp          = 0.0f,
        .depthBiasSlopeFactor    = 0.0f,
        .lineWidth               = 1.0f  // only needed for other polygon modes
    };

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable   = VK_FALSE,
        .minSampleShading      = 1.0f,
        .pSampleMask           = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable      = VK_FALSE};

    // TODO: when not using this, we just pass a nullptr to the pipeline creation instead of this struct
    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthTestEnable       = VK_FALSE,
        .depthWriteEnable      = VK_FALSE,
        .depthCompareOp        = VK_COMPARE_OP_NEVER,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable     = VK_FALSE,
        .front                 = VkStencilOpState(),
        .back                  = VkStencilOpState(),
        .minDepthBounds        = 0.0f,
        .maxDepthBounds        = 1.0f};

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState{
        .blendEnable         = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp        = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp        = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
                          | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable   = VK_FALSE,
        .logicOp         = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments    = &colorBlendAttachmentState,
        .blendConstants  = {0.0f, 0.0f, 0.0f, 0.0f}};

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                                        .pNext = nullptr,
                                                        .flags = 0,
                                                        .setLayoutCount = 0,
                                                        .pSetLayouts = nullptr,
                                                        .pushConstantRangeCount = 0,
                                                        .pPushConstantRanges = nullptr};

    VkPipelineLayout pipelineLayout;

    check<GraphicsPipelineCreationException>(
        vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout),
        "Pipeline Layout creation failed.");
    SLOG_INFO("Successfully created Pipeline Layout.");


    // shader module can only to be destroyed AFTER the pipeline creation is done
    // TODO: is there any point where we might have to reuse a shader module?
    //       -> if so, we could also add them to the class and cleanup in the end
    shaderModule.destroy(device);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    SLOG_INFO("Pipeline Layout cleanup complete.");
}