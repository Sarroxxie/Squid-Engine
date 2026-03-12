cmake_minimum_required(VERSION 3.24...4.2)
project(SquidEngine)

set(CMAKE_CXX_STANDARD 23)

add_executable(SquidEngine
src/main.cpp
src/application/Application.h
src/application/Application.cpp
src/application/Window.h
src/application/Window.cpp
src/setup/InstanceBuilder.h
src/setup/InstanceBuilder.cpp
src/application/DefaultApplication.h
src/application/DefaultApplication.cpp
src/setup/DebugUtilsMessenger.h
src/setup/DebugUtilsMessenger.cpp
src/setup/PhysicalDeviceSelector.h
src/setup/PhysicalDeviceSelector.cpp
src/setup/DefaultPhysicalDeviceSelector.h
src/setup/DefaultPhysicalDeviceSelector.cpp
src/setup/QueueFamiliyIndices.h
src/setup/QueueFamilyIndices.cpp
src/setup/DeviceBuilder.h
src/setup/DeviceBuilder.cpp
src/presentation/SwapChain.h
src/presentation/SwapChain.cpp)

# automatically sets the project as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SquidEngine)

# sets compiler to C++ 23
target_compile_features(SquidEngine PUBLIC cxx_std_23)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})

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
${PROJECT_ROOT_DIR}/src # include so local includes are relative to sources root
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