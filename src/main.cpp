#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <filesystem>
#include <iostream>
#include "pipeline/ShaderModule.h"
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
     * 2.5. write shader abstraction (autocompile can be added later on)
     * 3. implement graphics pipeline + render pass
     * 4. Hello Triangle
     * 5. add ImGui (maybe set context per Window)
     * 6. Shadertoy (use Text Editor Widget from ImGui?)
     *
     *
     * -> also split application and rendering! application should handle inputs
     * (and physics?)
     */

    try {
        ShaderModule module = ShaderModule(std::string(
            "F:/programming/Squid-Engine/shaders/slang/rainbow_triangle.slang"));
        module.compile();
    } catch(std::exception& e) {
        SLOG_FATAL(e.what());
        exit(EXIT_FAILURE);
    }

    Window window(WIDTH, HEIGHT, APP_NAME);

    DefaultVulkanRenderer renderer(&window, true);

    Application app(&renderer);

    app.run();

    app.cleanup();
    glfwTerminate();
    return 0;
}