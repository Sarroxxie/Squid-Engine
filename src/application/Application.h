#pragma once

#include "Window.h"
#include "presentation/Swapchain.h"

/*
This class keeps track of all Vulkan objects that are mandatory for all usecases.
This includes VkInstance, VkDevice, etc. It is an abstract class, so that instance
creation, device selection, etc. can be modified to fit specific needs.
 */
class Application
{
  public:
    Application(Window* const window);
    /*
    Initializes the application by calling all the pure virtual functions in the correct order.
    */
    virtual void init() final;
    VkInstance&  getInstance();
    /*
    Destroys all used resources.
    */
    void cleanup();

  protected:
    Window* const    window;
    VkInstance       instance       = VK_NULL_HANDLE;
    VkSurfaceKHR     surface        = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice         device         = VK_NULL_HANDLE;
    Swapchain        swapchain;


    // TODO: maybe there is a better location for these, in some larger class that handles rendering
    //       -> instead should separate "Application" and "Rendering" completely
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue  = VK_NULL_HANDLE;

  private:
    /*
    Creates the VkInstance. After the execution of this function, a valid
    VkInstance must be stored in the variable "instance".
    */
    virtual void createInstance() = 0;
    /*
    Creates the Window Surface. After the execution of this function, a valid
    VkSurfaceKHR must be stored in the variable "surface".
    */
    virtual void createSurface() = 0;
    /*
    Selects the Physical Device. After the execution of this function, a
    valid VkPhysicalDevice must be stored in the variable "physicalDevice".
    */
    virtual void selectPhysicalDevice() = 0;
    /*
    Creates the (logical) Device. After the execution of this function, a
    valid VkDevice must be stored in the variable "device".
    */
    virtual void createDevice() = 0;
    /*
    Creates the Swapchain. After the execution of this function, a valid
    VkSwapchainKHR must be stored in the variable "swapchain".
    */
    virtual void createSwapchain() = 0;
};