#pragma once

#include "Application.h"
#include "setup/DebugUtilsMessenger.h"
#include <vector>

// change this macro to false to disable validation layers
#define USE_DEBUG_UTILS true

/*
 This class serves as a default implementation for the abstract class
 "Application". Take it as a guide on how to implement your own Application.
 */
class DefaultApplication : public Application
{
  public:
    DefaultApplication();
    // Destroys all used resources.
    void cleanup();

  private:
    DebugUtilsMessenger debugMessenger;

    // Creates a VkInstance and registers validation layers if "USE_DEBUG_UTILS" is set to true.
    void initInstance();
    // Returns the instance extensions that are required by GLFW.
    std::vector<const char*> getRequiredExtensions();
};