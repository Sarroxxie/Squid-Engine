#pragma once

#include "Window.h"
#include "renderer/DefaultVulkanRenderer.h"

/*
This class keeps track of all Vulkan objects that are mandatory for all usecases.
This includes VkInstance, VkDevice, etc. It is an abstract class, so that instance
creation, device selection, etc. can be modified to fit specific needs.
 */
class Application
{
  public:
    Application(DefaultVulkanRenderer* const renderer);

    /*
    Destroys all used resources.
    */
    void cleanup();

    void run();

    // TODO: input should be rewired here (from the window) -> need to use callback data:
    //       (CallbackData*) glfwGetWindowUserPointer(renderer->getWindow()->getWindowHandle())
    void processFrame();

  protected:
    // TODO: find a way to keep this field generic, so any renderer can be inserted
    DefaultVulkanRenderer* const renderer;
};