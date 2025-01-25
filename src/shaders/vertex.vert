#version 450

#define USE_TEXTURE_BIT 0x01
#define USE_MSDF_BIT    0x02
#define TRANSPARENT_BIT 0x04
#define SCENE_BIT       0x08

#define MAX_SCENES 1024

layout(location = 0) in vec3 in_inPosition;
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

struct Scene {
    ivec2 size;
    ivec2 position;
    float cameraFieldOfView;
    vec3  cameraPosition;
    vec3  cameraRotation;
};

layout(binding = 0) uniform ScenesBuffer {
    Scene scenes[MAX_SCENES];
} sb;

mat3 rotateX(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat3(1.0, 0.0, 0.0,
                0.0, c,   -s,
                0.0, s,   c);
}

mat3 rotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat3(c,   0.0, s,
                0.0, 1.0, 0.0,
                -s,  0.0, c);
}

mat3 rotateZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat3(c,   -s,  0.0,
                s,   c,   0.0,
                0.0, 0.0, 1.0);
}

void main() {
    out_fragColor     = in_inColor.rgb;
    out_flags         = flags;
    out_textureNumber = textureNumber;
    out_textureCord   = in_textureCord;
    out_sceneNumber   = sceneNumber;

    int flags = int(flags);

    bool useScene = (flags & SCENE_BIT) != 0;

    if (useScene) {
        mat3 rotationX = rotateX(sb.scenes[sceneNumber].cameraRotation.x);
        mat3 rotationY = rotateY(sb.scenes[sceneNumber].cameraRotation.y);
        mat3 rotationZ = rotateZ(sb.scenes[sceneNumber].cameraRotation.z); 

        gl_Position = vec4(in_inPosition * rotationX * rotationY * rotationZ, 1.0);
    } else {
        gl_Position = vec4(in_inPosition, 1.0);
    }

    
}