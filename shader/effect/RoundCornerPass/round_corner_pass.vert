#version 450

layout (binding = 0) uniform GlobalInfo {
    mat4 view;
    mat4 proj;
} globalInfo;

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoord;

layout (location = 0) out vec2 texCoord;

void main()
{
    gl_Position = globalInfo.proj * globalInfo.view * vec4(vPos.x, vPos.y, 0.0, 1.0);
    texCoord = vTexCoord;
}