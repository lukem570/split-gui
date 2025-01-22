#version 450

#define USE_TEXTURE_BIT 0x01
#define USE_MSDF_BIT 0x02

struct Scene {
    ivec2 size;
    ivec2 position;
    float cameraFieldOfView;
    vec3  cameraPosition;
    vec3  cameraRotation;
};

layout(location = 0)      in vec3 in_fragColor;
layout(location = 1) flat in uint in_flags;
layout(location = 2) flat in uint in_textureNumber;
layout(location = 3)      in vec2 in_textureCord;

layout(binding = 1) uniform ScenesBuffer {
    Scene scenes[];
} sb;

layout(binding = 2) uniform sampler2DArray glyphs;

layout(location = 0) out vec4 outColor;

void main() {

    bool useTexture = in_flags == USE_MSDF_BIT;

    if (useTexture) {
        outColor = texture(glyphs, vec3(in_textureCord, in_textureNumber));
    } else {
        outColor = vec4(in_fragColor, 1.0);
    }
    
}