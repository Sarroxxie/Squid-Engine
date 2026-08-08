#pragma once

#include "VulkanRenderer.h"
#include "setup/DebugUtilsMessenger.h"
#include "presentation/SwapchainBuilder.h"
#include <vector>

/*
This class serves as a default implementation for the abstract class
"VulkanRenderer". Take it as a guide on how to implement your own VulkanRenderer.
 */
class DefaultVulkanRenderer : public VulkanRenderer
{
  public:
    // Creates a DefaultApplication without enabling validation layers.
    DefaultVulkanRenderer(Window* const window);
    // Creates a DefaultApplication with the option of enabling validation layers.
    DefaultVulkanRenderer(Window* const window, bool useDebugUtils);
    // Destroys all used resources.
    void cleanup() override;

  private:
    // whether to use validation layers
    const bool          USE_DEBUG_UTILS;
    DebugUtilsMessenger debugMessenger;
    SwapchainBuilder    swapchainBuilder;

    // Uses the InstanceBuilder to create the VkInstance and registers
    // validation layers if "USE_DEBUG_UTILS" is set to true.
    void createInstance();
    // Returns the instance extensions that are required by GLFW.
    std::vector<const char*> getRequiredExtensions();
    // Creates the Window Surface.
    void createSurface();
    // Uses the DefaultPhysicalDeviceSelector to select the most suitable GPU.
    void selectPhysicalDevice();
    // Uses the DeviceBuilder to create the VkDevice.
    void createDevice();
    // Uses the SwapchainBuilder to create the VkSwapchainKHR.
    void createSwapchain();
};