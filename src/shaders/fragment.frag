#version 450

layout(location = 0)      in vec3 in_fragColor;
layout(location = 1) flat in uint in_flags;
layout(location = 2) flat in uint in_textureNumber;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(in_fragColor, 1.0);
}