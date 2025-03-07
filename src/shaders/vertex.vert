#version 450 core

#define USE_TEXTURE_BIT 0x01
#define USE_MSDF_BIT    0x02
#define TRANSPARENT_BIT 0x04
#define SCENE_BIT       0x08
#define WORLD_VIEW_BIT  0x10

#define MAX_SCENES 1024

layout(location = 0) in vec3 in_inPosition;
layout(location = 1) in vec3 in_inColor;
layout(location = 2) in vec2 in_textureCord;
layout(location = 3) in uvec2 in_flags;
layout(location = 4) in uvec2 in_index;

uint flags = uint(in_flags.x);
uint index = uint(in_index.x);

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out uint out_flags;
layout(location = 2) out vec2 out_textureCord;
layout(location = 3) out uint out_index;

void main() {
    out_fragColor     = in_inColor.rgb;
    out_flags         = flags;
    out_textureCord   = in_textureCord;
    out_index         = index;

    vec4 pos = vec4(in_inPosition, 1.0f);

    gl_Position = pos;
}