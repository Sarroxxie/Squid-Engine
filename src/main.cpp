#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <filesystem>
#include "shader/ShaderModule.h"
#include "shader/CommandLineShaderCompiler.h"
#include "output/Logger.h"

// how to include the Vulkan Memory Allocator VMA (if it is installed with the
// VulkanSDK)
// #include <vma/vk_mem_alloc.h>

#include "application/Application.h"

constexpr int  WIDTH      = 1280;
constexpr int  HEIGHT     = 720;
constexpr char APP_NAME[] = "Squid Engine";

int main() {
    /* TODO: roadmap
     *
     * 3. implement graphics pipeline + render pass
     * 4. Hello Triangle
     * 4.5 multiple windows (includes multiple swapchains)
     *     -> "Application" can have multiple windows (but some Vulkan stuff has
     *        to be shared!)
     * 5. add ImGui (maybe set context per Window)
     * 6. Shadertoy Application (frag shader and/or compute shader)
     *
     *
     * -> also split application and rendering! application should handle inputs
     * (and physics?)
     */

    Window window(WIDTH, HEIGHT, APP_NAME);

    DefaultVulkanRenderer renderer(&window, true);

    Application app(&renderer);

    app.run();

    app.cleanup();
    glfwTerminate();
    return 0;
}