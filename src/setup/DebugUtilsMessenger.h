#pragma once

#include <vulkan/vulkan.h>

/*
This class is a wrapper for the VkDebugUtilsMessengerEXT. It simplifies creation
and destruction. The extension "VK_EXT_debug_utils" is required for a working
version, else an exception is thrown at creation.
*/
class DebugUtilsMessenger
{
  public:
    // TODO: the default constructor is only defined, so this class cannot be
    // used as an attribute in another class. It would be optimal if the default
    // constructor could never be called, but it is unclear to me at the moment,
    // if this is possible.

    // This will not create a valid DebugUtilsMessenger, as
    // creation requires a valid VkInstance.
    DebugUtilsMessenger() = default;
    // Creates a valid DebugUtilsMessenger. If the extension "VK_EXT_debug_utils"
    // is not supported, this constructor will throw an exception.
    DebugUtilsMessenger(VkInstance& instance);
    // Destroys all used resources.
    void cleanup(VkInstance& instance);
    // Fills in the create info for the debug utils messenger. The resulting
    // create info can also be attached to the InstanceBuilder so creation and
    // destruction of the instance can be debugged.
    static void populateDebugUtilsMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

  private:
    VkDebugUtilsMessengerEXT vkDebugMessenger = VK_NULL_HANDLE;
};