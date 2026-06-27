#pragma once

#include <stdexcept>

/*
Error messages are of format "prefix + connector + message"
The connector is shared for all exceptions while the prefix is custom per exception type.
*/

constexpr char CONNECTOR[] = ": ";

constexpr char INSTANCE_CREATION_PREFIX[]         = "Instance Creation";
constexpr char SURFACE_CREATION_PREFIX[]          = "Surface Creation";
constexpr char PHYSICAL_DEVICE_SELECTION_PREFIX[] = "Physical Device Selection";
constexpr char DEVICE_CREATION_PREFIX[]           = "Device Creation";
constexpr char SWAPCHAIN_CREATION_PREFIX[]        = "Swapchain Creation";
constexpr char DEBUG_UTILS_MESSENGER_CREATION_PREFIX[] = "Debug Utils Messenger Creation";
constexpr char QUEUE_FAMILY_INDEX_PREFIX[] = "Queue Family Index";

class InstanceCreationException : public std::runtime_error
{
  public:
    InstanceCreationException(const std::string& message)
        : std::runtime_error(
              (INSTANCE_CREATION_PREFIX + std::string(CONNECTOR) + message).c_str()) {};
};

class SurfaceCreationException : public std::runtime_error
{
  public:
    SurfaceCreationException(const std::string& message)
        : std::runtime_error(
              (SURFACE_CREATION_PREFIX + std::string(CONNECTOR) + message).c_str()) {};
};

class PhysicalDeviceSelectionException : public std::runtime_error
{
  public:
    PhysicalDeviceSelectionException(const std::string& message)
        : std::runtime_error((PHYSICAL_DEVICE_SELECTION_PREFIX + std::string(CONNECTOR) + message)
                                 .c_str()) {};
};

class DeviceCreationException : public std::runtime_error
{
  public:
    DeviceCreationException(const std::string& message)
        : std::runtime_error(
              (DEVICE_CREATION_PREFIX + std::string(CONNECTOR) + message).c_str()) {};
};

class SwapchainCreationException : public std::runtime_error
{
  public:
    SwapchainCreationException(const std::string& message)
        : std::runtime_error(
              (SWAPCHAIN_CREATION_PREFIX + std::string(CONNECTOR) + message).c_str()) {};
};

class DebugUtilsMessengerCreationException : public std::runtime_error
{
  public:
    DebugUtilsMessengerCreationException(const std::string& message)
        : std::runtime_error((DEBUG_UTILS_MESSENGER_CREATION_PREFIX + std::string(CONNECTOR) + message)
                                 .c_str()) {};
};

class QueueFamiliyIndexException : public std::runtime_error
{
  public:
    QueueFamiliyIndexException(const std::string& message)
        : std::runtime_error(
              (QUEUE_FAMILY_INDEX_PREFIX + std::string(CONNECTOR) + message).c_str()) {};
};