#pragma once

#include <vulkan/vulkan.h>

/*
This class should keep track of all Vulkan objects that are mandatory for all
usecases. This includes VkInstance, VkDevice, etc.
It is an abstract class, so that instance creation, device selection, etc.
can be modified to fit specific needs.
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
    // this makes the class abstract -> should be implemented for custom
    // applications (e.g. instance creation will be done here)
    virtual void init(){};
};