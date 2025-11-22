#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoord;

layout (location = 0) out vec2 texCoord;

void main()
{
    gl_Position = vec4(vPos.x, vPos.y, 0.0, 1.0);
    texCoord = vTexCoord;
}