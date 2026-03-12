#pragma once

#include "Application.h"
#include "setup/DebugUtilsMessenger.h"
#include <vector>

/*
 This class serves as a default implementation for the abstract class
 "Application". Take it as a guide on how to implement your own Application.
 */
class DefaultApplication : public Application
{
  public:
    // Creates a DefaultApplication without enabling validation layers.
    DefaultApplication(Window* window);
    // Creates a DefaultApplication with the option of enabling validation layers.
    DefaultApplication(Window* window, bool useDebugUtils);
    // Destroys all used resources.
    void cleanup();

  private:
    // whether to use validation layers
    const bool          USE_DEBUG_UTILS;
    DebugUtilsMessenger debugMessenger;

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
};