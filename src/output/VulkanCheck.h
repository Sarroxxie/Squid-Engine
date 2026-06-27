#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <type_traits>

/*
 * A few helper functions to throw exceptions when something goes wrong.
 */

template <typename T>
void check(const VkResult result);
template <typename T>
void check(const VkResult result, const std::string message);

template <typename T>
void check(const bool result);
template <typename T>
void check(const bool result, const std::string message);


// these will default to a std::runtime_error
void check(const VkResult result);
void check(const VkResult result, const std::string message);

void check(const bool result);
void check(const bool result, const std::string message);

/*
TODO: find a nice solution to separate definition and implementation of template
functions into separate files
template functions will have to be implemented inside the .h file for
now... see:
https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
*/

template <typename T>
void check(const VkResult result) {
    static_assert(std::is_base_of<std::exception, T>::value,
                  "T must inherit from std::exception (in VulkanCheck.h).");
    if(result != VK_SUCCESS)
        throw T("Vulkan call returned an error.\n");
}

template <typename T>
void check(const VkResult result, const std::string message) {
    static_assert(std::is_base_of<std::exception, T>::value,
                  "T must inherit from std::exception (in VulkanCheck.h).");
    if(result != VK_SUCCESS)
        throw T(message);
}

template <typename T>
void check(const bool result) {
    static_assert(std::is_base_of<std::exception, T>::value,
                  "T must inherit from std::exception (in VulkanCheck.h).");
    if(!result)
        throw T("Call returned an error.\n");
}

template <typename T>
void check(const bool result, const std::string message) {
    static_assert(std::is_base_of<std::exception, T>::value,
                  "T must inherit from std::exception (in VulkanCheck.h).");
    if(!result)
        throw T(message);
}