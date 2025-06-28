cmake_minimum_required(VERSION 3.29)
project(ShaderCompiler)

set(PATH_BIN "$ENV{VULKAN_SDK}/bin")
set(GLSL_LANG_VALIDATOR "${PATH_BIN}/glslangValidator")
set(PATH_SHADER_INPUT "../../shader")
set(PATH_SHADER_OUTPUT "../../src/shader/vulkan")
set(PATH_SHADER_OUTPUT_TEMP "${CMAKE_SOURCE_DIR}/output/vulkan")

include(cmake/CompileShaderToHeader.cmake)

COMPILE_ALL_SHADER(${PATH_SHADER_INPUT} ${PATH_SHADER_OUTPUT_TEMP})
TRANSFORM_ALL_SPV(${PATH_SHADER_OUTPUT_TEMP} ${PATH_SHADER_OUTPUT})