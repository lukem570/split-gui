#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 2) in uvec2 in_flags;
layout(location = 3) in uvec2 in_sceneNumber;
layout(location = 4) in uvec2 in_textureNumber;

uint flags         = uint(in_flags.x);
uint sceneNumber   = uint(in_sceneNumber.x);
uint textureNumber = uint(in_textureNumber.x);

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out uint out_flags;
layout(location = 2) out uint out_textureNumber;

layout(binding = 0) uniform UniformBufferObject {
    ivec2 screenSize;
} ubo;

void main() {
    gl_Position       = vec4(inPosition, 0.0, 1.0);
    out_fragColor     = inColor.rgb;
    out_flags         = flags;
    out_textureNumber = textureNumber;

    // need to apply rotation matrices depending on the scene

}