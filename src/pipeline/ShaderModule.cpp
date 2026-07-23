#include "ShaderModule.h"

#include "output/Logger.h"
#include "output/VulkanCheck.h"
#include "Exceptions.h"

// the _WIN32 macro includes 64-bit systems as well
#ifdef _WIN32
constexpr char DIRECTORY_SEPARATOR[] = "\\";
#else
constexpr char DIRECTORY_SEPARATOR[] = "/";
#endif

ShaderModule::ShaderModule() {}

ShaderModule::ShaderModule(std::filesystem::path sourceFilePath) {
    setSourceFilePath(sourceFilePath);
}

ShaderModule::ShaderModule(std::string sourceFilePath)
    : ShaderModule(std::filesystem::path(sourceFilePath)) {}

void ShaderModule::setSourceFilePath(std::filesystem::path sourceFilePath) {
    // choose either backslashes (on Windows) or forward slashes (on POSIX)
    std::filesystem::path path = sourceFilePath.make_preferred();

    if(path.is_absolute()) {
        // check if the shader file exists
        check<ShaderCreationException>(std::filesystem::exists(path),
                                       "Shader source file \"" + path.string() + "\" does not exist.");

        // check if the shader file lies within the shader source directory
        std::filesystem::path relativePath =
            std::filesystem::relative(path, SHADER_SOURCE_DIRECTORY_PATH);

        check<ShaderCreationException>(!relativePath.empty(),
                                       "Shader source file \"" + path.string() + "\" does not exist within the shader source directory ("
                                           + SHADER_SOURCE_DIRECTORY_PATH + ").");

        this->sourceFilePath = relativePath;
    } else {
        // check if the shader file exists
        check<ShaderCreationException>(
            std::filesystem::exists(std::filesystem::path(SHADER_SOURCE_DIRECTORY_PATH) / path),
            "Shader source file \"" + path.string() + "\" does not exist in the shader source directory ("
                + SHADER_SOURCE_DIRECTORY_PATH + ").");

        this->sourceFilePath = path;
    }
}

void ShaderModule::setSourceFilePath(std::string sourceFilePath) {
    setSourceFilePath(std::filesystem::path(sourceFilePath));
}

void ShaderModule::compile() {
    std::string command =
        "slangc " + std::string(SHADER_SOURCE_DIRECTORY_PATH)
        + DIRECTORY_SEPARATOR + sourceFilePath.string() + " -target spirv -o "
        + std::string(SHADER_COMPILE_DIRECTORY_PATH) + std::string(DIRECTORY_SEPARATOR)
        + sourceFilePath.parent_path().string() + sourceFilePath.stem().string() + ".spv";


    SLOG_INFO("Compiling Shader \"" << sourceFilePath.string() << "\"");
    system(command.c_str());
}

void ShaderModule::destroy(VkDevice& device) {
    if(handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, handle, nullptr);
        SLOG_INFO("Destroyed Shader " << sourceFilePath.string());
    }
}