#pragma once

#include "PhysicalDeviceSelector.h"

class DefaultPhysicalDeviceSelector : public PhysicalDeviceSelector
{
  private:
    // Preferes dedicated GPUs.
    int rateDeviceSuitability(const VkPhysicalDevice& device);
};