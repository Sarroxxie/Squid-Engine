#pragma once

#include <stdexcept>

// TODO: should standardize this (especially formatting) to be easily adjustable for all future expections

class SwapchainCreationException : public std::runtime_error
{
  public:
    SwapchainCreationException(std::string message)
        : message(MESSAGE_PREFIX + std::string(": ") + message)
        , std::runtime_error(message) {}
    const char* what() const override { return message.c_str(); }

  private:
    static constexpr char MESSAGE_PREFIX[] = "Swapchain Creation Exception";
    std::string           message;
};