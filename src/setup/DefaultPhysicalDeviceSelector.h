#pragma once

#include "PhysicalDeviceSelector.h"

class DefaultPhysicalDeviceSelector : public PhysicalDeviceSelector
{
  protected:
    /*
    Preferes dedicated GPUs.
    */
    int rateDeviceSuitability(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) const;
};