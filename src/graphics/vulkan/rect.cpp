#include "vulkan.hpp"

namespace SplitGui {
    RectRef VulkanInterface::drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth, VertexFlags flags, uint16_t textureIndex) {

        int verticesOffset = vertices.size();
        int indicesOffset = indices.size();

        indices.push_back(verticesOffset + 2);
        indices.push_back(verticesOffset + 0);
        indices.push_back(verticesOffset + 3);
        indices.push_back(verticesOffset + 3);
        indices.push_back(verticesOffset + 0);
        indices.push_back(verticesOffset + 1);
        
        Vertex bottomLeft;
        bottomLeft.pos         = {std::min(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        bottomLeft.color       = color;
        bottomLeft.textureCord = {0.0f, 0.0f};

        Vertex bottomRight;
        bottomRight.pos         = {std::max(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        bottomRight.color       = color;
        bottomRight.textureCord = {1.0f, 0.0f};

        Vertex topLeft;
        topLeft.pos         = {std::min(x1.x, x2.x), std::max(x1.y, x2.y), depth};
        topLeft.color       = color;
        topLeft.textureCord = {0.0f, 1.0f};

        Vertex topRight;
        topRight.pos         = {std::max(x1.x, x2.x), std::max(x1.y, x2.y), depth};
        topRight.color       = color;
        topRight.textureCord = {1.0f, 1.0f};

        vertices.resize(verticesOffset + 4);

        vertices[verticesOffset + 0] = {bottomLeft,  flags, textureIndex};
        vertices[verticesOffset + 1] = {bottomRight, flags, textureIndex};
        vertices[verticesOffset + 2] = {topLeft,     flags, textureIndex};
        vertices[verticesOffset + 3] = {topRight,    flags, textureIndex};

        RectRef refRet;
        refRet.bottomLeft   = verticesOffset + 0;
        refRet.bottomRight  = verticesOffset + 1;
        refRet.topLeft      = verticesOffset + 2;
        refRet.topRight     = verticesOffset + 3;
        refRet.indicesStart = indicesOffset;
        refRet.id           = rectReferences.size();

        rectOffsets.push_back(0);
        rectReferences.push_back(refRet);

        Logger::info("Created Rect: " + std::to_string(refRet.bottomLeft));

        return refRet;
    }

    void VulkanInterface::updateRect(RectRef& ref, Vec2 x1, Vec2 x2, Vec3 color, float depth) {

        unsigned int offset = rectOffsets[ref.id];

        vertices[offset + ref.bottomLeft].vertex.pos  = {std::min(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        vertices[offset + ref.bottomRight].vertex.pos = {std::max(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        vertices[offset + ref.topLeft].vertex.pos     = {std::min(x1.x, x2.x), std::max(x1.y, x2.y), depth};
        vertices[offset + ref.topRight].vertex.pos    = {std::max(x1.x, x2.x), std::max(x1.y, x2.y), depth};

        vertices[offset + ref.bottomLeft].vertex.color  = color;
        vertices[offset + ref.bottomRight].vertex.color = color;
        vertices[offset + ref.topLeft].vertex.color     = color;
        vertices[offset + ref.topRight].vertex.color    = color;

        markVerticesForUpdate = true;
    }

    void VulkanInterface::deleteRect(RectRef& ref) {

        for (unsigned int i = 0; i < rectReferences.size(); i++) {
            if (ref.bottomLeft < rectReferences[i].bottomLeft) {
                rectOffsets[i] -= 4;

                for (unsigned int j = 0; j < 6; j++){
                    indices[rectReferences[i].indicesStart + j] -= 4;
                }
            }
        }

        vertices.erase(
            vertices.begin() + ref.bottomLeft, 
            vertices.begin() + ref.topRight
        );

        indices.erase(
            indices.begin() + ref.indicesStart, 
            indices.begin() + ref.indicesStart
        );
    }

    Result VulkanInterface::submitRect(RectRef& ref) {

        unsigned int offset = rectOffsets[ref.id];

        vk::DeviceSize rectSize = sizeof(VertexBufferObject) * 4;

        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingBufferRes, createBuffer(
            rectSize, 
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            stagingBuffer,
            stagingBufferMemory
        ));

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, rectSize);

        std::memcpy(memory, (char*)vertices.data() + sizeof(VertexBufferObject) * (offset + ref.bottomLeft), rectSize);

        vk_device.unmapMemory(stagingBufferMemory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;
        copyRegion.size      = rectSize;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = sizeof(VertexBufferObject) * (offset + ref.bottomLeft);

        commandBuffer.copyBuffer(stagingBuffer, vk_vertexBuffer, 1, &copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        return Result::eSuccess;
    }
}