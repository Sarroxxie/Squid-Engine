#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// how to include the Vulkan Memory Allocator VMA (if it is installed with the
// VulkanSDK) 
// #include <vma/vk_mem_alloc.h>

#include "application/Window.h"
#include "application/DefaultApplication.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr char APP_NAME[] = "ThesisRenderer";

int main() {
    /* TODO: roadmap
     *
     * 2. implement swap chain
     * 3. implement graphics pipeline + render pass
     * 4. Hello Triangle
     * 5. add ImGui (maybe set context per Window)
     * 6. Shadertoy (use Text Editor Widget from ImGui?)
     */

    Window window(WIDTH, HEIGHT, APP_NAME);

    DefaultApplication app(&window);

    while(!window.shouldClose()) {
        // main loop
        glfwPollEvents();
    }
    window.~Window();

    app.cleanup();
    glfwTerminate();
    return 0;
}