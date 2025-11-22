#version 450
layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 FragColor;

layout(set = 0, binding = 0) uniform LocalInfo {
    vec2 iResolution;
    vec2 newMousePos;
    vec2 oldMousePos;
    int iFrame;
    int isPressed;
} localInfo;

layout(set = 0, binding = 1) uniform sampler2D uvReadTex;

float lineDist(vec2 p, vec2 start, vec2 end, float width)
{
    vec2 dir = start - end;
    float lngth = length(dir);
    dir /= lngth;
    vec2 proj = max(0.0, min(lngth, dot((start - p), dir))) * dir;
    return length((start - p) - proj) - width * 0.5;
}

void main() {
    vec2 uv = texCoord;
    vec2 col = uv;

    if (localInfo.iFrame == 0) {
        col = texture(uvReadTex, uv).xy;

        vec2 mouse    = localInfo.newMousePos.xy / localInfo.iResolution.xy;
        vec2 p_mouse  = localInfo.oldMousePos / localInfo.iResolution;
        float pressed = localInfo.isPressed;

        if (pressed > 0.0) {
            float d = lineDist(uv, mouse, p_mouse, 0.0) * 20.0;
            float w = clamp(1.0 - d, 0.0, 1.0) * 0.7;
            col = texture(uvReadTex, uv + (p_mouse - mouse) * w).xy;
        }
    }

    FragColor = vec4(col, 0.0, 1.0);
}