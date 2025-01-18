#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 2) in uvec2 flags_in;
layout(location = 3) in uvec2 sceneNumber_in;
layout(location = 4) in uvec2 textureNumber_in;

uint flags         = uint(flags_in.x);
uint sceneNumber   = uint(sceneNumber_in.x);
uint textureNumber = uint(textureNumber_in.x);

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out uint out_flags;
layout(location = 2) out uint out_textureNumber;

void main() {
    gl_Position       = vec4(inPosition, 0.0, 1.0);
    out_fragColor     = inColor.rgb;
    out_flags         = flags;
    out_textureNumber = textureNumber;

    // need to apply rotation matrices depending on the scene

}