#version 450

#define USE_TEXTURE_BIT 0x01
#define USE_MSDF_BIT    0x02
#define TRANSPARENT_BIT 0x04
#define SCENE_BIT       0x08

#define pxRange    6
#define MAX_SCENES 1024

struct Scene {
    ivec2 size;
    ivec2 position;
    mat4  cameraView;
    mat4  cameraProjection;
};

layout(location = 0)      in vec3 in_fragColor;
layout(location = 1) flat in uint in_flags;
layout(location = 2) flat in uint in_textureNumber;
layout(location = 3)      in vec2 in_textureCord;
layout(location = 4) flat in uint in_sceneNumber;
layout(location = 5)      in vec3 in_fragNorm;
layout(location = 6)      in vec3 in_fragPos;

layout(binding = 0) uniform ScenesBuffer {
    Scene scenes[MAX_SCENES];
} sb;

layout(binding = 1) uniform sampler2DArray glyphs;

layout(location = 0) out vec4 outColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(glyphs, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(in_textureCord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main() {

    int flags = int(in_flags);

    bool useMsdf  = (flags & USE_MSDF_BIT) != 0;
    bool useScene = (flags & SCENE_BIT)    != 0;

    if (useMsdf) {
        vec4 msdf = texture(glyphs, vec3(in_textureCord, in_textureNumber));
        float sd = median(msdf.r, msdf.g, msdf.b);
        float screenPxDistance = screenPxRange()*(sd - 0.5);
        float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
        outColor = mix(vec4(0.0), vec4(in_fragColor, 1.0), opacity);

        //outColor = vec4(texture(glyphs, vec3(in_textureCord, in_textureNumber)).rgb, 1.0);

    } else if (useScene) {

        Scene scene = sb.scenes[in_sceneNumber];

        bool leftBound = scene.position.x <= gl_FragCoord.x;
        bool topBound  = scene.position.y <= gl_FragCoord.y;

        bool rightBound  = scene.position.x + scene.size.x >= gl_FragCoord.x;
        bool bottomBound = scene.position.y + scene.size.y >= gl_FragCoord.y;

        if (leftBound && topBound && rightBound && bottomBound) {

            vec3 normal = normalize(in_fragNorm);

            vec3 lightPos = vec3(2.0, -1.0, 2.0);

            vec3 lightDir = normalize(lightPos - in_fragPos);

            float diffuse = max(dot(normal, lightDir), 0.1);

            outColor = vec4(diffuse * in_fragColor, 1.0);
        } else {
            outColor = vec4(0.0);
        }

    } else {
        outColor = vec4(in_fragColor, 1.0);
    }
    
}