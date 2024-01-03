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
    // Initializes the application by calling all the pure virtual functions in the correct order.
    void        init();
    VkInstance& getInstance();
    // Destroys all used resources.
    void cleanup();

  protected:
    VkInstance       instance;
    VkPhysicalDevice physicalDevice;

    bool validInstance = false;

  private:
    // Creates the VkInstance. After this function is done, either a valid VkInstance
    // is stored in the "instance" variable or an exception is thrown.
    virtual void createInstance() = 0;
    // Selects the Physical Device. After this function is done, either a valid
    // VkPhysicalDevice is stored in the "physicalDevice" variable or an exception is thrown.
    virtual void selectPhysicalDevice() = 0;
};