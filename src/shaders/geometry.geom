#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec3 in_fragColor[];
layout(location = 0) out vec3 out_fragColor;

//layout(binding = 0) uniform UniformBufferObject {
//    int   indexData;
//    ivec4 viewport;
//} ubo;

void main() {

    //int textureMapping =  ubo.indexData        & 0xFFFF;
    //int sceneNumber    = (ubo.indexData >> 16) & 0xFFFF;


    for (int i = 0; i < 3; ++i) {
        out_fragColor = in_fragColor[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}