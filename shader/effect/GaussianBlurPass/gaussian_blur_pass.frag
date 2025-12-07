#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform sampler2D targetTex;

layout(set = 0, binding = 1) uniform LocalInfo {
    float radius;         // blur radius in pixels (>=0)
    int direction;       // 0 horizontal, 1 vertical
} localInfo;

void main()
{
    float sigma = localInfo.radius;
    int support = int(sigma * 3.0);
    vec2 texSize = vec2(textureSize(targetTex, 0));
    vec2 loc   = vUV;         // center pixel cooordinate
    vec2 dir;
    if(localInfo.direction == 0) {
        dir = vec2(1.0/texSize.x, 0.0);
    }
    else {
        dir = vec2(0.0, 1.0/texSize.y);
    }
    vec4 acc   = vec4( 0.0 );                      // accumulator
    float norm = 0.0;
    for (int i = -support; i <= support; i++) {
        float coeff = exp(-0.5 * float(i) * float(i) / (sigma * sigma));
        acc += (texture(targetTex, loc + float(i) * dir)) * coeff;
        norm += coeff;
    }
    acc *= 1.0/norm;                               // normalize for unity gain

    // Output to screen
    fragColor = acc;
}