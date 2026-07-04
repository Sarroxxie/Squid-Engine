#pragma once

#include <stdexcept>

/*
Usage of exceptions: Choose a ClassName that describes WHERE (while doing which
step) the error occurred and use the message to describe WHAT exactly went wrong.
Example: ClassName = SwapchainCreationException
         message   = "Failed to create Image View after fetching Swapchain Image."
*/

/*
Error messages are of format "ClassName" + CONNECTOR + message
The connector is shared for all exceptions.
*/

constexpr char CONNECTOR[] = ": ";


/*
Use this Macro to generate different classes of exceptions (that extend
"std::runtime_error") and prepend their name as well as the "CONNECTOR" to the
error message. "ExceptionName" MUST be valid as a class name (use CamelCase)!
*/
#define GENERATE_EXCEPTION(ExceptionName)                                          \
    class ExceptionName : public std::runtime_error                                \
    {                                                                              \
      public:                                                                      \
        ExceptionName(const std::string& message)                                  \
            : std::runtime_error(                                                  \
                  (#ExceptionName + std::string(CONNECTOR) + message).c_str()) {}; \
    }

GENERATE_EXCEPTION(InstanceCreationException);
GENERATE_EXCEPTION(SurfaceCreationException);
GENERATE_EXCEPTION(PhysicalDeviceSelectionException);
GENERATE_EXCEPTION(DeviceCreationException);
GENERATE_EXCEPTION(SwapchainCreationException);
GENERATE_EXCEPTION(DebugUtilsMessengerCreationException);
GENERATE_EXCEPTION(QueueFamiliyIndexException);