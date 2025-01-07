#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in int  sceneNumber;

layout(location = 0) out vec3 out_fragColor;
layout(location = 1) out int out_sceneNumber;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    out_fragColor = inColor;

    // need to apply rotation matrices depending on the scene

    out_sceneNumber = sceneNumber;
}