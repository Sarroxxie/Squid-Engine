#include "SwapChain.h"

uint32_t SwapChain::getWidth() const {
    return extent.width;
}

uint32_t SwapChain::getHeight() const {
    return extent.height;
}

VkExtent2D SwapChain::getExtent() const {
    return extent;
}
