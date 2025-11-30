#version 450
#extension GL_GOOGLE_include_directive:enable
#define MAX_TEXTURE_COUNT 1

layout (location = 0) in vec2 texCoord;
layout (location = 1) flat in int textureIndex;
layout (location = 2) flat in int needAliasing;
layout (location = 3) in vec2 texSize;

layout (binding = 2) uniform sampler2D screenTex[MAX_TEXTURE_COUNT];
layout (location = 0) out vec4 FragColor;

float getAntiAliasingAlpha(vec2 size)
{
    float alpha= 1.;
    float aliasCount = 2;
    vec2 antiSize = vec2(aliasCount/size.x, aliasCount/size.y);
    vec2 texCoo = abs(texCoord - vec2(.5));

    float alphax= 1.- smoothstep(.5 - antiSize.x, .5,texCoo.x);
    float alphay= 1.- smoothstep(.5 - antiSize.y, .5,texCoo.y);

    return alphax * alphay;
}

void main()
{
    vec4 color;
    switch(textureIndex) {
        case 0: color = texture(screenTex[0], texCoord);    break;
        // case 1: color = texture(screenTex[1], texCoord);    break;
        // case 2: color = texture(screenTex[2], texCoord);    break;
        // case 3: color = texture(screenTex[3], texCoord);    break;
        // case 4: color = texture(screenTex[4], texCoord);    break;
        // case 5: color = texture(screenTex[5], texCoord);    break;
        // case 6: color = texture(screenTex[6], texCoord);    break;
        // case 7: color = texture(screenTex[7], texCoord);    break;
        // case 8: color = texture(screenTex[8], texCoord);    break;
        // case 9: color = texture(screenTex[9], texCoord);    break;
        // case 10: color = texture(screenTex[10], texCoord);  break;
        // case 11: color = texture(screenTex[11], texCoord);  break;
        // case 12: color = texture(screenTex[12], texCoord);  break;
        // case 13: color = texture(screenTex[13], texCoord);  break;
        // case 14: color = texture(screenTex[14], texCoord);  break;
        // case 15: color = texture(screenTex[15], texCoord);  break;
        default: color = vec4(1.0, 0.0, 0.0, 1.0);          break;
    }
    FragColor = vec4(color.rgb, needAliasing == 1 ? color.a * getAntiAliasingAlpha(texSize) : color.a);
}