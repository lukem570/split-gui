#version 450 core

#define USE_TEXTURE_BIT 0x01
#define USE_MSDF_BIT    0x02
#define TRANSPARENT_BIT 0x04
#define SCENE_BIT       0x08
#define WORLD_VIEW_BIT  0x10

#define MAX_MODELS 1024

layout(location = 0) in vec3  in_inPosition;
layout(location = 1) in vec3  in_inColor;
layout(location = 2) in vec2  in_textureCord;
layout(location = 3) in uvec2 in_flags;
layout(location = 4) in uvec2 in_textureNumber;
layout(location = 5) in uvec2 in_modelNumber;
layout(location = 6) in vec3  in_normal;

uint flags         = uint(in_flags.x);
uint textureNumber = uint(in_textureNumber.x);
uint modelNumber   = uint(in_modelNumber.x);

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out uint out_flags;
layout(location = 2) out uint out_textureNumber;
layout(location = 3) out vec2 out_textureCord;
layout(location = 4) out vec3 out_fragNorm;
layout(location = 5) out vec3 out_fragPos;

layout(std140, binding = 0) uniform Scene {
    mat4  view;
    mat4  projection;
    vec3  cameraPosition;
} sb;

layout(std140, binding = 1) uniform Model {mat4 models[MAX_MODELS];};

void main() {
    out_fragColor     = in_inColor.rgb;
    out_flags         = flags;
    out_textureNumber = textureNumber;
    out_textureCord   = in_textureCord;
    out_fragPos       = in_inPosition;
    out_fragNorm      = in_normal;

    int flags = int(flags);

    bool worldView = (flags & WORLD_VIEW_BIT) != 0;

    vec4 pos = vec4(in_inPosition, 1.0f);

    if (worldView) {
        pos += vec4(sb.cameraPosition.x, 0.0, sb.cameraPosition.z, 0.0);
    }

    pos -= vec4(sb.cameraPosition, 0.0);

    pos *= models[modelNumber];
    pos *= sb.view;
    pos *= sb.projection;

    gl_Position = pos;
}