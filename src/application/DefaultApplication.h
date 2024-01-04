#pragma once

#include "Application.h"
#include "setup/DebugUtilsMessenger.h"
#include <vector>

// change this value to false to disable validation layers
constexpr bool USE_DEBUG_UTILS = true;

/*
 This class serves as a default implementation for the abstract class
 "Application". Take it as a guide on how to implement your own Application.
 */
class DefaultApplication : public Application
{
  public:
    DefaultApplication(Window* window);
    // Destroys all used resources.
    void cleanup();

  private:
    DebugUtilsMessenger debugMessenger;

    // Uses the InstanceBuilder to create a VkInstance and registers validation
    // layers if "USE_DEBUG_UTILS" is set to true.
    void createInstance();
    // Returns the instance extensions that are required by GLFW.
    std::vector<const char*> getRequiredExtensions();
    // TODO: add docs
    void createSurface();
    // Uses the DefaultPhysicalDeviceSelector to select the most suitable GPU.
    void selectPhysicalDevice();
    // Uses the DeviceBuilder to create a VkDevice.
    void createDevice();
};