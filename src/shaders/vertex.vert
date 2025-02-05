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
    vec3  cameraRotation;
    vec3  cameraPosition;
};

layout(binding = 0) uniform ScenesBuffer {
    Scene scenes[MAX_SCENES];
} sb;

layout(binding = 2) uniform VertexUniform {
    ivec2 screenSize;
} vub;

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

float conformToScene(float imin, float imax, float x) {
    float m = (imin - imax) / -2.0f;
    float b = imax - m;
    return m * x + b;
}

void main() {
    out_fragColor     = in_inColor.rgb;
    out_flags         = flags;
    out_textureNumber = textureNumber;
    out_textureCord   = in_textureCord;
    out_sceneNumber   = sceneNumber;

    int flags = int(flags);

    bool useScene = (flags & SCENE_BIT) != 0;

    vec3 pos = in_inPosition;

    if (useScene) {
        Scene scene = sb.scenes[sceneNumber];

        // transform points around position
        pos -= scene.cameraPosition;

        mat3 rotationX = rotateX(scene.cameraRotation.x);
        mat3 rotationY = rotateY(scene.cameraRotation.y);
        mat3 rotationZ = rotateZ(scene.cameraRotation.z); 

        // rotate points
        pos *= rotationX * rotationY * rotationZ;

        // adjust points for aspect ratio
        if (scene.size.x > scene.size.y) {
            pos.x *= float(scene.size.y) / float(scene.size.x);
        } else {
            pos.y *= float(scene.size.x) / float(scene.size.y);
        }

        // conform points to viewport
        float iminx = float(scene.position.x) / float(vub.screenSize.x) * 2.0f - 1.0f;
        float imaxx = float(scene.size.x + scene.position.x) / float(vub.screenSize.x) * 2.0f - 1.0f;

        float iminy = float(scene.position.y) / float(vub.screenSize.y) * 2.0f - 1.0f;
        float imaxy = float(scene.size.y + scene.position.y) / float(vub.screenSize.y) * 2.0f - 1.0f;

        pos.x = conformToScene(iminx, imaxx, pos.x);
        pos.y = conformToScene(iminy, imaxy, pos.y);
    }

    gl_Position = vec4(pos.xy, 0.0, 1.0);
}