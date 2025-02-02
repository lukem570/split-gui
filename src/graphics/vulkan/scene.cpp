#include "vulkan.hpp"

namespace SplitGui {
    unsigned int VulkanInterface::instanceScene(IVec2 x1, IVec2 x2) {
        SceneObj scene;
        scene.viewport.width    = std::abs(x1.x - x2.x);
        scene.viewport.height   = std::abs(x1.y - x2.y);
        scene.viewport.x        = std::min(x1.x,  x2.x);
        scene.viewport.y        = std::min(x1.y,  x2.y);
        scene.cameraRotation    = {0, 0, 0};

        printf("scene: (%d, %d, %d, %d) (%d, %d) (%d, %d)\n", scene.viewport.x, scene.viewport.y, scene.viewport.width, scene.viewport.height, x1.x, x1.y, x2.x, x2.y);

        scenes.push_back(scene);
        
        return scenes.size() - 1;
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

    void VulkanInterface::updateScene(unsigned int ref, IVec2 x1, IVec2 x2) {
        scenes[ref].viewport.width    = std::abs(x1.x - x2.x);
        scenes[ref].viewport.height   = std::abs(x1.y - x2.y);
        scenes[ref].viewport.x        = std::min(x1.x,  x2.x);
        scenes[ref].viewport.y        = std::min(x1.y,  x2.y);

        markScenesForUpdate = true;
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
        vk_graphicsQueue.waitIdle();

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        updateScenes();

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position) {

        //scenes[sceneNumber].cameraPosition = position;

        return Result::eInvalidSetting;

        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, InstanceStagingBuffer(scenes, stagingBuffer, stagingBufferMemory, vk_sceneBufferSize));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  vk_sceneBuffer, vk_sceneBufferSize,  commandBuffer, copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));
        vk_graphicsQueue.waitIdle();

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        updateScenes();

        return Result::eSuccess;
    }
}