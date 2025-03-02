#version 450

#define USE_TEXTURE_BIT 0x01
#define USE_MSDF_BIT    0x02
#define TRANSPARENT_BIT 0x04
#define SCENE_BIT       0x08
#define WORLD_VIEW_BIT  0x10

#define pxRange    6
#define MAX_SCENES 1024

const float gridCellSize = 0.25;
const float gridFalloffRadius = 15;

struct Scene {
    ivec2 size;
    ivec2 position;
    mat4  cameraView;
    mat4  cameraProjection;
    vec3  cameraPosition;
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
layout(binding = 3) uniform sampler2DArray textures;

layout(location = 0) out vec4 outColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {

    int flags = int(in_flags);

    bool useMsdf    = (flags & USE_MSDF_BIT) != 0;
    bool useTexture = (flags & USE_TEXTURE_BIT) != 0;
    bool useScene   = (flags & SCENE_BIT)    != 0;
    bool worldView  = (flags & WORLD_VIEW_BIT) != 0;

    if (useMsdf && !useScene) {
        vec4 msdf;

        if (useTexture) {
            msdf = texture(textures, vec3(in_textureCord, in_textureNumber));
        } else {
            msdf = texture(glyphs, vec3(in_textureCord, in_textureNumber));
        }

        float sd = median(msdf.r, msdf.g, msdf.b);
        float w = fwidth(sd);
        float opacity = smoothstep(0.5 - w, 0.5 + w, sd);

        outColor = mix(vec4(in_fragColor, 0.0), vec4(in_fragColor, 1.0), opacity);

    } else if (useScene) {

        Scene scene = sb.scenes[in_sceneNumber];

        bool leftBound = scene.position.x <= gl_FragCoord.x;
        bool topBound  = scene.position.y <= gl_FragCoord.y;

        bool rightBound  = scene.position.x + scene.size.x >= gl_FragCoord.x;
        bool bottomBound = scene.position.y + scene.size.y >= gl_FragCoord.y;

        if (leftBound && topBound && rightBound && bottomBound) {

            if (useTexture && useMsdf) {

                vec4 msdf = texture(textures, vec3(in_textureCord, in_textureNumber));

                float sd = median(msdf.r, msdf.g, msdf.b);
                float w = fwidth(sd);
                float opacity = smoothstep(0.5 - w, 0.5 + w, sd);

                outColor = mix(vec4(in_fragColor, 0.0), vec4(in_fragColor, 1.0), opacity);

            } else if (!worldView) {
                vec3 normal = normalize(in_fragNorm);

                vec3 lightPos = vec3(2.0, -5.0, 3.0);

                vec3 lightDir = normalize(lightPos - in_fragPos);

                float diffuse = max(dot(normal, lightDir), 0.1);

                outColor = vec4(diffuse * in_fragColor, 1.0);
            } else {

                vec2 pos = in_fragPos.xz ;

                vec2 dvx = vec2(dFdx(pos.x), dFdy(pos.x));
                vec2 dvy = vec2(dFdx(pos.y), dFdy(pos.y));

                float lx = length(dvx);
                float ly = length(dvy);

                vec2 dudv = vec2(lx, ly);

                dudv *= 4.0f;

                vec2 modCell =  mod(pos+ scene.cameraPosition.xz , gridCellSize) / dudv;
                vec2 modDiv = vec2(1.0) - abs(clamp(modCell, vec2(0.0), vec2(1.0)) * 2.0 - vec2(1.0));

                float Lod0 = max(modDiv.x, modDiv.y);

                vec2 height = pos;

                float falloff = max(0.0, -1.0 / (gridFalloffRadius * gridFalloffRadius) * (height.x * height.x + height.y * height.y) + 1.0);

                outColor = vec4(in_fragColor, Lod0 * falloff);
            }

        } else {
            outColor = vec4(0.0);
        }

    } else {
        outColor = vec4(in_fragColor, 1.0);
    }
    
}