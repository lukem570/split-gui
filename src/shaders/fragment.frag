#version 450

#define USE_TEXTURE_BIT    0x01
#define USE_MSDF_BIT       0x02
#define TRANSPARENT_BIT    0x04
#define SCENE_BIT          0x08
#define WORLD_VIEW_BIT     0x10
#define ENABLE_CROP_REGION 0x20

struct CropRegion {
    ivec2 x1;
    ivec2 x2;
};

layout(location = 0)      in vec4 in_fragColor;
layout(location = 1) flat in uint in_flags;
layout(location = 2)      in vec2 in_textureCord;
layout(location = 3) flat in uint in_index;
layout(location = 4) flat in uint in_cropIndex;

layout(binding = 0) uniform sampler2DArray glyphs;
layout(binding = 1) uniform sampler2DArray scenes;
layout(binding = 2) uniform sampler2DArray textures;

layout(std140, binding = 3) uniform CropRegionData {
    CropRegion regions[1024];
} cr;

layout(location = 0) out vec4 outColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {

    int flags = int(in_flags);

    bool useMsdf       = (flags & USE_MSDF_BIT) != 0;
    bool useTexture    = (flags & USE_TEXTURE_BIT) != 0;
    bool useScene      = (flags & SCENE_BIT) != 0;
    bool useCropRegion = (flags & ENABLE_CROP_REGION) != 0;

    if (useCropRegion) {
        if (
            min(cr.regions[in_cropIndex].x1.x, cr.regions[in_cropIndex].x2.x) > gl_FragCoord.x ||
            max(cr.regions[in_cropIndex].x1.x, cr.regions[in_cropIndex].x2.x) < gl_FragCoord.x ||
            min(cr.regions[in_cropIndex].x1.y, cr.regions[in_cropIndex].x2.y) > gl_FragCoord.y ||
            max(cr.regions[in_cropIndex].x1.y, cr.regions[in_cropIndex].x2.y) < gl_FragCoord.y)
            discard;
    }

    if (useMsdf && !useScene) {
        vec4 msdf;

        if (useTexture) {
            msdf = texture(textures, vec3(in_textureCord, in_index));
        } else {
            msdf = texture(glyphs, vec3(in_textureCord, in_index));
        }

        float sd = median(msdf.r, msdf.g, msdf.b);
        float w = fwidth(sd);
        float opacity = smoothstep(0.5 - w, 0.5 + w, sd);

        outColor = mix(vec4(in_fragColor.rgb, 0.0), vec4(in_fragColor.rgb, 1.0), opacity);

    } else if (useTexture) {

        outColor = texture(textures, vec3(in_textureCord, in_index)) * in_fragColor;
        
    } else if (useScene) {

        outColor = texture(scenes, vec3(in_textureCord, in_index));

    } else {
        outColor = in_fragColor;
    }
    
    if (outColor.a == 0) {
        discard;
    }
}