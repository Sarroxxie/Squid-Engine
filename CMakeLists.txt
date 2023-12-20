cmake_minimum_required(VERSION 3.24)
project(ThesisRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThesisRenderer src/main.cpp)

# automatically sets the project as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ThesisRenderer)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})

# Constants for lib includes
set(GLFW_DIR libs/glfw/)
set(IMGUI_DIR libs/imgui/)
set(GLM_DIR libs/glm/)
set(VKBOOTSTRAP_DIR libs/vk-bootstrap/)

# TODO: see if we need this for model loading
# set(TINY_GLTF_DIR libs/tiny_gltf/)

# VulkanSDK
find_package(Vulkan REQUIRED)

# ImGUI
target_include_directories(ThesisRenderer PUBLIC
${PROJECT_ROOT_DIR}/src # include so local includes are relative to sources root
${IMGUI_DIR} 
${IMGUI_DIR}/backends
)

target_sources(ThesisRenderer PUBLIC 
"${IMGUI_DIR}/imgui.h"
"${IMGUI_DIR}/imgui.cpp"
"${IMGUI_DIR}/imgui_draw.cpp"
"${IMGUI_DIR}/imgui_tables.cpp"
"${IMGUI_DIR}/imgui_widgets.cpp"
"${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp"
"${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
)

# We only use glfw, no need to test it or build the docs for us
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add and link libs
add_subdirectory(${GLFW_DIR})
target_link_libraries(ThesisRenderer glfw ${GLFW_LIBRARIES})

target_link_libraries(ThesisRenderer Vulkan::Vulkan)

add_subdirectory(${GLM_DIR})
target_link_libraries(ThesisRenderer glm)

add_subdirectory(${VKBOOTSTRAP_DIR})
target_link_libraries(ThesisRenderer vk-bootstrap::vk-bootstrap)