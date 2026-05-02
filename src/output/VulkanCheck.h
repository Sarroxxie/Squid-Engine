#pragma once

#include <vulkan/vulkan.h>
#include <string>

/*
 * A few helper functions to throw exceptions when something goes wrong.
 */

void check(const VkResult result);
void check(const VkResult result, const std::string message);

void check(const bool result);
void check(const bool result, const std::string message);