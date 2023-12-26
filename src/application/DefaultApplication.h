#pragma once

#include "Application.h"
#include <vector>

/*
 This class serves as a default implementation for the abstract class
 "Application". Take it as a guide on how to implement your own Application.
 */
class DefaultApplication : public Application
{
  public:
    DefaultApplication();

  private:
    void init();

    // Returns the instance extensions that are required by GLFW.
    std::vector<const char*> getRequiredExtensions();
};