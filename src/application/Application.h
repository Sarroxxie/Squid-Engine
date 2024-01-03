#pragma once

#include <vulkan/vulkan.h>

/*
This class keeps track of all Vulkan objects that are mandatory for all usecases.
This includes VkInstance, VkDevice, etc. It is an abstract class, so that instance
creation, device selection, etc. can be modified to fit specific needs.
 */
class Application
{
  public:
    VkInstance& getInstance();
    // Destroys all used resources.
    void cleanup();

  protected:
    VkInstance instance;
    bool       validInstance = false;

  private:
    // Takes care of Vulkan Instance creation.
    virtual void initInstance(){};
};