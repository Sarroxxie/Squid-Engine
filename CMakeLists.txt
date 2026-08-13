cmake_minimum_required(VERSION 3.25...4.2)
project(SquidEngine)

set(CMAKE_CXX_STANDARD 23)

add_executable(SquidEngine
src/main.cpp
src/Exceptions.h

src/application/Application.h
src/application/Application.cpp
src/application/Window.h
src/application/Window.cpp

src/renderer/VulkanRenderer.h
src/renderer/VulkanRenderer.cpp
src/renderer/DefaultVulkanRenderer.h
src/renderer/DefaultVulkanRenderer.cpp

src/setup/DebugUtilsMessenger.h
src/setup/DebugUtilsMessenger.cpp
src/setup/DefaultPhysicalDeviceSelector.h
src/setup/DefaultPhysicalDeviceSelector.cpp
src/setup/DeviceBuilder.h
src/setup/DeviceBuilder.cpp
src/setup/InstanceBuilder.h
src/setup/InstanceBuilder.cpp
src/setup/PhysicalDeviceSelector.h
src/setup/PhysicalDeviceSelector.cpp
src/setup/QueueFamiliyIndices.h
src/setup/QueueFamilyIndices.cpp

src/shader/ShaderCompiler.h
src/shader/ShaderCompiler.cpp
src/shader/ShaderStageBuilder.h
src/shader/ShaderStageBuilder.cpp
src/shader/ShaderModule.h
src/shader/ShaderModule.cpp

src/presentation/Swapchain.h
src/presentation/Swapchain.cpp
src/presentation/SwapchainBuilder.h
src/presentation/SwapchainBuilder.cpp
src/presentation/ImageViewBuilder.h
src/presentation/ImageViewBuilder.cpp

src/output/VulkanCheck.h
src/output/VulkanCheck.cpp
src/output/Logger.h
)

# automatically sets the project as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SquidEngine)

# sets compiler to C++ 23
target_compile_features(SquidEngine PUBLIC cxx_std_23)

# Constants for lib includes
set(GLFW_DIR libs/glfw/)
set(IMGUI_DIR libs/imgui/)
set(GLM_DIR libs/glm/)
set(VKBOOTSTRAP_DIR libs/vk-bootstrap/)

# TODO: see if we want to use this for model loading
# set(TINY_GLTF_DIR libs/tiny_gltf/)

# VulkanSDK
find_package(Vulkan REQUIRED)

# ImGUI
target_include_directories(SquidEngine PUBLIC
${PROJECT_SOURCE_DIR}/src # include so local includes are relative to sources root
${IMGUI_DIR}
${IMGUI_DIR}/backends
)

target_sources(SquidEngine PUBLIC 
${IMGUI_DIR}/imgui.h
${IMGUI_DIR}/imgui.cpp
${IMGUI_DIR}/imgui_draw.cpp
${IMGUI_DIR}/imgui_tables.cpp
${IMGUI_DIR}/imgui_widgets.cpp
${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

# We only use glfw, no need to test it or build the docs for us (see https://www.glfw.org/docs/3.3/build_guide.html#:~:text=With%20CMake%20and%20GLFW%20source,-This)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add and link libs
add_subdirectory(${GLFW_DIR})
target_link_libraries(SquidEngine glfw ${GLFW_LIBRARIES})

target_link_libraries(SquidEngine Vulkan::Vulkan)

add_subdirectory(${GLM_DIR})
target_link_libraries(SquidEngine glm)

add_subdirectory(${VKBOOTSTRAP_DIR})
target_link_libraries(SquidEngine vk-bootstrap::vk-bootstrap)

# Create directory for compiled shaders
file(MAKE_DIRECTORY "./shaders/spv") #TODO: should store the path somewhere as it is also being used later!

# changes connectors to use forward slashes in paths
function(to_posix_path PATH)
    string(REGEX REPLACE "\\\\" "/" ${PATH} ${${PATH}})
    return(PROPAGATE ${PATH})
endfunction()

# changes connectors to use backslashes in paths
function(to_windows_path PATH)
    string(REGEX REPLACE "/" "\\\\\\\\" ${PATH} ${${PATH}})
    return(PROPAGATE ${PATH})
endfunction()

# uses slashes for posix paths and backslashes for windows paths
function(make_path_platform_dependent PATH)
    to_posix_path(${PATH})
    IF (WIN32)
        to_windows_path(${PATH})
    ENDIF()
    return(PROPAGATE ${PATH})
endfunction()

# NOTE: For some reason, if using the "to_windows_path" function for an ending slash, cmake appends 
#       a different number of backslashes than for slashes that are not at the end of the string.
#       This function is designed to be used AFTER "make_path_platform_dependent" has been called!
function(append_platform_dependent_connector PATH)
    if (WIN32)
	    string(APPEND ${PATH} "\\\\")
	ELSE()
	    string(APPEND ${PATH} "/")
	ENDIF()
	return(PROPAGATE ${PATH})
endfunction()

# Pass various paths to the programm through compile macros
block()
set(VulkanSDKPath $ENV{VULKAN_SDK})
    make_path_platform_dependent(VulkanSDKPath)
	append_platform_dependent_connector(VulkanSDKPath)
    target_compile_definitions(SquidEngine PRIVATE "VULKAN_SDK_PATH=\"${VulkanSDKPath}\"")
endblock()

block()
    set(ProjectRoot ${PROJECT_SOURCE_DIR})
    make_path_platform_dependent(ProjectRoot)
	append_platform_dependent_connector(ProjectRoot)
    target_compile_definitions(SquidEngine PRIVATE "PROJECT_ROOT_PATH=\"${ProjectRoot}\"")
endblock()

block()
    set(ShaderSourceDirectory ${PROJECT_SOURCE_DIR})
    string(APPEND ShaderSourceDirectory "/shaders/slang")
    make_path_platform_dependent(ShaderSourceDirectory)
	append_platform_dependent_connector(ShaderSourceDirectory)
    target_compile_definitions(SquidEngine PRIVATE "SHADER_SOURCE_DIRECTORY_PATH=\"${ShaderSourceDirectory}\"")
endblock()

block()
    set(ShaderCompileDirectory ${PROJECT_SOURCE_DIR})
    string(APPEND ShaderCompileDirectory "/shaders/spv")
    make_path_platform_dependent(ShaderCompileDirectory)
	append_platform_dependent_connector(ShaderCompileDirectory)
    target_compile_definitions(SquidEngine PRIVATE "SHADER_COMPILE_DIRECTORY_PATH=\"${ShaderCompileDirectory}\"")
endblock()