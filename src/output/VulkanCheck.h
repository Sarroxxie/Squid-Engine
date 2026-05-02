#pragma once

#include <vulkan/vulkan.h>
#include <string>

/*
 * A few helper functions to throw exceptions when something goes wrong.
 */

void check(VkResult result);
void check(VkResult result, std::string message);

void check(bool result);
void check(bool result, std::string message);