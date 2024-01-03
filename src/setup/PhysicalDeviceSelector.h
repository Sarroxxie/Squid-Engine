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
    // Finds the GPU that is most suitable. An expection is thrown if no suitable GPU is found.
    VkPhysicalDevice& selectPhysicalDevice(VkInstance& instance);

  private:
    // Rates the suitability of the GPU. Higher score means better suitable,
    // negative score means not suitable at all.
    virtual int rateDeviceSuitability(const VkPhysicalDevice& device) = 0;
};