#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::instanceScene(IVec2 x1, IVec2 x2) {
        SceneObj scene;
        scene.viewport.width    = std::abs(x1.x - x2.x);
        scene.viewport.height   = std::abs(x1.y - x2.y);
        scene.viewport.x        = std::min(x1.x,  x2.x);
        scene.viewport.y        = std::min(x1.y,  x2.y);
        scene.cameraPosition    = {0, 0, 0};
        scene.cameraRotation    = {0, 0, 0};
        scene.cameraFieldOfView = 90;

        printf("scene: (%d, %d, %d, %d) (%d, %d) (%d, %d)", scene.viewport.x, scene.viewport.y, scene.viewport.width, scene.viewport.height, x1.x, x1.y, x2.x, x2.y);

        scenes.push_back(scene);
    }

    void VulkanInterface::submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& newVertices, std::vector<uint16_t>& newIndices) {
        int oldVerticesSize = vertices.size();
        int oldIndicesSize  = indices.size();

        vertices.resize(oldVerticesSize + newVertices.size());
        indices.resize(oldIndicesSize + newIndices.size());

        for (int i = 0; i < newVertices.size(); i++) {
            VertexBufferObject vbo;
            vbo.vertex        = newVertices[i];
            vbo.flags         = VertexFlagsBits::eScene;
            vbo.sceneNumber   = sceneNumber;
            vbo.textureNumber = 0;

            vertices[oldVerticesSize + i] = vbo;
        }

        for (int i = 0; i < newIndices.size(); i++) {
            indices[oldIndicesSize + i] = oldVerticesSize + newIndices[i];
        }

        printf("submitted triangles\n");
    }
}