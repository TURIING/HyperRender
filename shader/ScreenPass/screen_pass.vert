#version 450
#extension GL_GOOGLE_include_directive:enable

#include "GlobalInfo.chunk"

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec2 iOffset;
layout (location = 3) in vec2 iSize;
layout (location = 4) in int iTextureIndex;

layout (location = 0) out vec2 texCoord;
layout (location = 1) flat out int textureIndex;

layout (binding = 1) uniform LocalInfo {
    vec2 screenSize;
} localInfo;

void main()
{
    vec2 local = (vPos + vec2(1.0)) * 0.5;                  // 将vPos [-1, 1]转换到[0,1]区域内
    vec2 newPos = iOffset + local * iSize;                  // 得到真实的欲贴图的目标区域
    vec2 ndc = newPos / localInfo.screenSize * 2.0 - 1.0;
    gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0);
    texCoord = vTexCoord;
    textureIndex = iTextureIndex;
}