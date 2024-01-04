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
    virtual void init() final;
    VkInstance&  getInstance();
    // Destroys all used resources.
    void cleanup();

  protected:
    VkInstance       instance;
    VkPhysicalDevice physicalDevice;
    VkDevice         device;

  private:
    // Creates the VkInstance. After the execution of this function, a valid
    // VkInstance must be stored in the variable "instance".
    virtual void createInstance() = 0;
    // Selects the Physical Device. After the execution of this function, a
    // valid VkPhysicalDevice must be stored in the variable "physicalDevice".
    virtual void selectPhysicalDevice() = 0;
    // Creates the (logical) Device. After the execution of this function, a
    // valid VkDevice must be stored in the variable "device".
    virtual void createDevice() = 0;

    // This documentation is needed to clean only those resources that were
    // created. Important for cases where some part of the initialization fails.
    bool validInstance = false;
    bool validDevice   = false;
};