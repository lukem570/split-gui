#include "vulkan.hpp"

namespace SplitGui {
    RectRef VulkanInterface::drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth, VertexFlags flags, uint16_t textureIndex) {
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < rectReferences.size(); i++) {
            if (ref.bottomLeft < rectReferences[i].bottomLeft) {
                rectOffsets[i] -= 4;
            }
        }

        for (unsigned int i = 0; i < indices.size(); i++) {
            if (ref.topRight < indices[i]) {
                indices[i] -= 4;
            }
        }
        

        vertices.erase(
            vertices.begin() + ref.bottomLeft, 
            vertices.begin() + ref.topRight + 1
        );

        indices.erase(
            indices.begin() + ref.indicesStart, 
            indices.begin() + ref.indicesStart + 6
        );
    }

    Result VulkanInterface::submitRect(RectRef& ref) {
        SPLITGUI_PROFILE;

        unsigned int offset = rectOffsets[ref.id];

        TRYR(stagingRes, InstanceStagingBuffer(vk_rectStagingBuffer, sizeof(VertexBufferObject) * 4));
        
        void* memory = vk_device.mapMemory(vk_rectStagingBuffer.memory, 0, vk_rectStagingBuffer.size);

        std::memcpy(memory, (char*)vertices.data() + sizeof(VertexBufferObject) * (offset + ref.bottomLeft), vk_rectStagingBuffer.size);

        vk_device.unmapMemory(vk_rectStagingBuffer.memory);

        vk::CommandBuffer commandBuffer = startCopyBuffer(vk_rectStagingBuffer);

        vk::BufferCopy copyRegion;
        copyRegion.size      = vk_rectStagingBuffer.size;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = sizeof(VertexBufferObject) * (offset + ref.bottomLeft);

        commandBuffer.copyBuffer(vk_rectStagingBuffer.buffer, vk_vertexBuffer, 1, &copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        return Result::eSuccess;
    }
}