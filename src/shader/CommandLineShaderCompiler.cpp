#include "CommandLineShaderCompiler.h"

#include "output/Logger.h"

// the _WIN32 macro includes 64-bit systems as well
#ifdef _WIN32
constexpr char DIRECTORY_SEPARATOR[] = "\\";
#else
constexpr char DIRECTORY_SEPARATOR[] = "/";
#endif

bool CommandLineShaderCompiler::compileShader(ShaderModule& shaderModule) {
    // TODO: need to generalize the command

    /*
    example command (see
    https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/01_Shader_modules.html):

    slangc shader.slang -target spirv -profile spirv_1_4 -emit-spirv-directly
           -fvk-use-entrypoint-name -entry vertMain -entry fragMain -o slang.spv
    */

    // compile command
    std::string command = "slangc";

    // input file
    command += " " + std::string(SHADER_SOURCE_DIRECTORY_PATH)
               + DIRECTORY_SEPARATOR + shaderModule.getSourceFilePath().string();

    // specify spirv as target
    command += " -target spirv -profile spirv_1_4 -emit-spirv-directly";

    // add entry points
    command += " -fvk-use-entrypoint-name";
    for(std::string entryPoint : shaderModule.getEntryPoints()) {
        command += " -entry " + entryPoint;
    }

    // specify output file
    command += " -o " + std::string(SHADER_COMPILE_DIRECTORY_PATH)
               + std::string(DIRECTORY_SEPARATOR)
               + shaderModule.getSourceFilePath().parent_path().string()
               + shaderModule.getSourceFilePath().stem().string() + ".spv";


    SLOG_INFO("Compiling Shader \"" << shaderModule.getSourceFilePath().string() << "\"");
    int result = system(command.c_str());

    if(result == 0) {
        return true;
    }
    SLOG_ERROR("Compilation of Shader \""
                 << shaderModule.getSourceFilePath().string() << "\" failed.");
    return false;
}