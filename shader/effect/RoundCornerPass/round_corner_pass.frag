#version 450

layout (binding = 1) uniform sampler2D screenTex;
layout (location = 0) out vec4 FragColor;
layout (binding = 2) uniform LocalInfo {
    vec2 resolution;
} localInfo;

//float sdRoundRect(vec2 p, vec2 b, float r) {
//    vec2 q = abs(p) - b + r;
//    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
//}

void main()
{
    //    vec2 uv = gl_FragCoord.xy / localInfo.resolution;
    //    vec2 center = localInfo.resolution.xy / 2.0;
    //    vec2 hsize = localInfo.resolution.xy / 4.0;
    //
    //    vec4 texColor = texture(screenTex, uv);
    //
    //    float a = clamp(sdRoundRect(vec2(gl_FragCoord.xy - center), hsize, 50.0), 0.0, 1.0);
    //
    //    vec4 col = vec4(1.0 - a);
    //
    //    texColor *= col;
    //
    //    // Output to screen
    //    FragColor = texColor;
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}