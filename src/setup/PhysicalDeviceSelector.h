#pragma once

#include <vulkan/vulkan.h>

/*
This class is a wrapper for selecting a suitable physical device (GPU). For
custom requirements, extend this class and implement the
"rateDeviceSuitability(..)" method and add your requirements there.
*/
class PhysicalDeviceSelector
{
  public:
    /*
    Finds the GPU that is most suitable. An expection is thrown if no suitable GPU is found.
    */
    const VkPhysicalDevice& selectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface) const;

  protected:
    /*
    Rates the suitability of the GPU. Will return a negative score if the
    QueueFamilyIndices defined in "QueueFamilyIndices.h" are not found.
    */
    int rateDeviceSuitability(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) const;
};