#version 450
#extension GL_GOOGLE_include_directive:enable
#include "ColorFunc.chunk"

layout (location = 0) in vec2 texCoord;
layout (location = 0) out vec4 outColor;
layout (binding = 0) uniform sampler2D uTex;

layout(set = 0, binding = 1) uniform LocalInfo {
    float azimuth;                 // 角度，单位：弧度
    float elevation;               // 弧度
    float bumpHeight;
    int useGray;                   // 是否使用灰度表示
} localInfo;

void main()
{
    vec2 uv = texCoord;
    vec2 texSize = vec2(textureSize(uTex, 0));
    vec2 texel = 1.0 / texSize;
    float width45 = localInfo.bumpHeight * 3;

    // 光线方向（整数版转为浮点版）
    float Lx =  cos(localInfo.azimuth) * cos(localInfo.elevation);
    float Ly =  sin(localInfo.azimuth) * cos(localInfo.elevation);
    float Lz =  sin(localInfo.elevation);

    float Nz  = 6.0 / width45;
    float Nz2 = Nz * Nz;
    float NzLz = Nz * Lz;

    float background = Lz;

    // 采样九宫格亮度（高度图）
    // 跳过边界
    ivec2 xy = ivec2(gl_FragCoord.xy);
    if (xy.x <= 0 || xy.y <= 0 || xy.x >= int(texSize.x)-1 || xy.y >= int(texSize.y)-1) {
        float shade = background;
        outColor = vec4(vec3(shade), 1.0);
        return;
    }

    float TL = brightness(texture(uTex, uv + texel * vec2(-1, -1)).rgb);      // 左上
    float  L = brightness(texture(uTex, uv + texel * vec2(-1,  0)).rgb);      // 左
    float BL = brightness(texture(uTex, uv + texel * vec2(-1,  1)).rgb);      // 左下

    float TC = brightness(texture(uTex, uv + texel * vec2( 0, -1)).rgb);      // 中上
    float BC = brightness(texture(uTex, uv + texel * vec2( 0,  1)).rgb);      // 中下

    float TR = brightness(texture(uTex, uv + texel * vec2( 1, -1)).rgb);      // 右上
    float  R = brightness(texture(uTex, uv + texel * vec2( 1,  0)).rgb);      // 右
    float BR = brightness(texture(uTex, uv + texel * vec2( 1,  1)).rgb);      // 右下

    // Nx = (左列) - (右列)
    // Nx 核（水平方向）:
    // [ +1   0  -1 ]
    // [ +1   0  -1 ]
    // [ +1   0  -1 ]
    float Nx = (TL + L + BL) - (TR + R + BR);

    // Ny = (下排) - (上排)
    // Ny 核:（垂直方向差分）
    // [ +1  +1  +1 ]
    // [  0   0   0 ]
    // [ -1  -1  -1 ]
    float Ny = (BL + BC + BR) - (TL + TC + TR);

    float shade;
    if (Nx == 0.0 && Ny == 0.0) {
        shade = background;
    } else {
        float NdotL = Nx * Lx + Ny * Ly + NzLz;
        if (NdotL < 0.0) {
            shade = 0.0;            // 点乘结果为负数，说明法线方向与光线方向相反
        }
        else {
            shade = NdotL;
        }
    }

    float s = clamp(shade, 0.0, 1.0);
    if(localInfo.useGray == 1) {
        outColor = vec4(vec3(s), 1.0);
    }
    else {
        vec3 src = texture(uTex, uv).rgb;
        outColor = vec4(src * s, 1.0);
    }
}