#include "vulkan.hpp"

namespace SplitGui {
    unsigned int VulkanInterface::instanceScene(IVec2 x1, IVec2 x2) {
        SceneObj scene;
        scene.viewport.width    = std::abs(x1.x - x2.x);
        scene.viewport.height   = std::abs(x1.y - x2.y);
        scene.viewport.x        = std::min(x1.x,  x2.x);
        scene.viewport.y        = std::min(x1.y,  x2.y);
        scene.cameraRotation    = {0, 0, 0};
        scene.cameraPosition    = {0, 0, 0};

        SPLITGUI_LOG("Created Scene: %ld", scenes.size());

        scenes.push_back(scene);
        
        return scenes.size() - 1;
    }

    void VulkanInterface::submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& newVertices, std::vector<uint16_t>& newIndices) {
        unsigned int oldVerticesSize = vertices.size();
        unsigned int oldIndicesSize  = indices.size();

        vertices.resize(oldVerticesSize + newVertices.size());
        indices.resize(oldIndicesSize + newIndices.size());

        for (unsigned int i = 0; i < newVertices.size(); i++) {
            VertexBufferObject vbo;
            vbo.vertex        = newVertices[i];
            vbo.flags         = VertexFlagsBits::eScene;
            vbo.sceneNumber   = sceneNumber;
            vbo.textureNumber = 0;

            vertices[oldVerticesSize + i] = vbo;
        }

        for (unsigned int i = 0; i < newIndices.size(); i++) {
            indices[oldIndicesSize + i] = oldVerticesSize + newIndices[i];
        }

        SPLITGUI_LOG("Submitted Triangles: Verts= %ld, Indices= %ld, Total Verts= %ld, Total Indices= %ld", newVertices.size(), newIndices.size(), vertices.size(), indices.size());
    }

    void VulkanInterface::updateScene(unsigned int ref, IVec2 x1, IVec2 x2) {
        scenes[ref].viewport.width    = std::abs(x1.x - x2.x);
        scenes[ref].viewport.height   = std::abs(x1.y - x2.y);
        scenes[ref].viewport.x        = std::min(x1.x,  x2.x);
        scenes[ref].viewport.y        = std::min(x1.y,  x2.y);

        markScenesForUpdate = true;

        SPLITGUI_LOG("Updated Scene: %d", ref);
    }

    Result VulkanInterface::updateSceneCameraRotation(unsigned int sceneNumber, Vec3& rotation) {

        scenes[sceneNumber].cameraRotation = rotation;

        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, InstanceStagingBuffer(scenes, stagingBuffer, stagingBufferMemory, vk_sceneBufferSize));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  vk_sceneBuffer, vk_sceneBufferSize,  commandBuffer, copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        updateScenes();

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position) {

        scenes[sceneNumber].cameraPosition = position;

        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, InstanceStagingBuffer(scenes, stagingBuffer, stagingBufferMemory, vk_sceneBufferSize));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  vk_sceneBuffer, vk_sceneBufferSize,  commandBuffer, copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        updateScenes();

        return Result::eSuccess;
    }
}