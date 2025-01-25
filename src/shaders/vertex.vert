#version 450

layout(location = 0) in vec2 in_inPosition;
layout(location = 1) in vec3 in_inColor;
layout(location = 2) in vec2 in_textureCord;

layout(location = 3) in uvec2 in_flags;
layout(location = 4) in uvec2 in_sceneNumber;
layout(location = 5) in uvec2 in_textureNumber;

uint flags         = uint(in_flags.x);
uint sceneNumber   = uint(in_sceneNumber.x);
uint textureNumber = uint(in_textureNumber.x);

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out uint out_flags;
layout(location = 2) out uint out_textureNumber;
layout(location = 3) out vec2 out_textureCord;
layout(location = 4) out uint out_sceneNumber;

layout(binding = 0) uniform UniformBufferObject {
    ivec2 screenSize;
} ubo;

void main() {
    gl_Position       = vec4(in_inPosition, 0.0, 1.0);
    out_fragColor     = in_inColor.rgb;
    out_flags         = flags;
    out_textureNumber = textureNumber;
    out_textureCord   = in_textureCord;
    out_sceneNumber   = sceneNumber;

    // need to apply rotation matrices depending on the scene

}