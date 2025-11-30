#version 450
#extension GL_GOOGLE_include_directive:enable

#include "GlobalInfo.chunk"

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec4 iModel1;
layout (location = 3) in vec4 iModel2;
layout (location = 4) in vec4 iModel3;
layout (location = 5) in vec4 iModel4;
layout (location = 6) in vec2 iOffset;
layout (location = 7) in vec2 iSize;
layout (location = 8) in int iTextureIndex;

layout (location = 0) out vec2 texCoord;
layout (location = 1) flat out int textureIndex;

layout (binding = 1) uniform LocalInfo {
    vec2 screenSize;
} localInfo;

void main()
{
    vec2 newPos = iOffset + vPos * iSize;
    mat4 model = mat4(iModel1, iModel2, iModel3, iModel4);
    gl_Position = globalInfo.proj * model * vec4(newPos, 0.0, 1.0);
    gl_Position.y = -gl_Position.y;
    texCoord = vTexCoord;
    textureIndex = iTextureIndex;
}