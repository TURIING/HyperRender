#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 texCoord;
layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D uTex;

layout(set = 0, binding = 1) uniform LocalInfo {
    float radius;           // 旋转半径，px
    float angle;            // 旋转角度，弧度制
    vec2 center;            // 旋转中心像素坐标
} localInfo;

void main()
{
    vec2 texSize = vec2(textureSize(uTex, 0));
    float effectRadius = localInfo.radius / texSize.y;
    float effectAngle = localInfo.angle;

    vec2 center = localInfo.center / texSize;
    vec2 uv = texCoord - center;                // 以旋转中心为原点

    float len = length(uv * vec2(texSize.x / texSize.y, 1.));                               // 对宽高比做限制，防止出现椭圆
    float angle = atan(uv.y, uv.x) + effectAngle * smoothstep(effectRadius, 0., len);       // 极坐标的角度θ
    float radius = length(uv);                                                              // 极坐标的半径r
    vec2 cood = vec2(radius * cos(angle), radius * sin(angle)) + center;                    // 极坐标转回成笛卡尔坐标
    FragColor = texture(uTex, cood);
}