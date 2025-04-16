#include "vulkan.hpp"

namespace SplitGui {
    RectRef VulkanInterface::drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth, VertexFlags flags, uint16_t textureIndex) {
        SPLITGUI_PROFILE;

        int verticesOffset = vertices.size();

        LinkElement<uint16_t>* iStart = indices.push(verticesOffset + 2);

        indices.push(verticesOffset + 0);
        indices.push(verticesOffset + 3);
        indices.push(verticesOffset + 3);
        indices.push(verticesOffset + 0);

        LinkElement<uint16_t>* iEnd = indices.push(verticesOffset + 1);
        
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

        LinkElement<VertexBufferObject>* bottomLeftElement  = vertices.push({bottomLeft,  flags, textureIndex});
        LinkElement<VertexBufferObject>* bottomRightElement = vertices.push({bottomRight, flags, textureIndex});
        LinkElement<VertexBufferObject>* topLeftElement     = vertices.push({topLeft,     flags, textureIndex});
        LinkElement<VertexBufferObject>* topRightElement    = vertices.push({topRight,    flags, textureIndex});

        RectRef refRet;
        refRet.indicesStart  = iStart;
        refRet.indicesEnd    = iEnd;
        
        refRet.bottomLeft  = bottomLeftElement;
        refRet.bottomRight = bottomRightElement;
        refRet.topLeft     = topLeftElement;
        refRet.topRight    = topRightElement;

        Logger::info("Created Rect: " + std::to_string(vertices.size()));

        return refRet;
    }

    void VulkanInterface::updateRect(RectRef& ref, Vec2 x1, Vec2 x2, Vec3 color, float depth) {
        SPLITGUI_PROFILE;

        ref.bottomLeft->data.vertex.pos  = {std::min(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        ref.bottomRight->data.vertex.pos = {std::max(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        ref.topLeft->data.vertex.pos     = {std::min(x1.x, x2.x), std::max(x1.y, x2.y), depth};
        ref.topRight->data.vertex.pos    = {std::max(x1.x, x2.x), std::max(x1.y, x2.y), depth};

        ref.bottomLeft->data.vertex.color  = color;
        ref.bottomRight->data.vertex.color = color;
        ref.topLeft->data.vertex.color     = color;
        ref.topRight->data.vertex.color    = color;

        markVerticesForUpdate = true;
    }

    void VulkanInterface::deleteRect(RectRef& ref) {
        SPLITGUI_PROFILE;

        vertices.erase(ref.verticesStart, ref.verticesEnd);
        indices.erase(ref.indicesStart, ref.indicesEnd);
    }

    Result VulkanInterface::submitRect(RectRef& ref) {
        SPLITGUI_PROFILE;

        TRYR(stagingRes, InstanceStagingBuffer(vk_rectStagingBuffer, sizeof(VertexBufferObject) * 4));
        
        void* memory = vk_device.mapMemory(vk_rectStagingBuffer.memory, 0, vk_rectStagingBuffer.size);

        std::optional<unsigned int> rectOff = vertices.offset(ref.verticesStart);

        if (!rectOff.has_value()) {
            return Result::eInvalidRectRefUsed;
        }

        std::memcpy(memory, vertices.sliceArray(ref.verticesStart, ref.verticesEnd), vk_rectStagingBuffer.size);

        vk_device.unmapMemory(vk_rectStagingBuffer.memory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;
        copyRegion.size      = vk_rectStagingBuffer.size;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = sizeof(VertexBufferObject) * rectOff.value();

        commandBuffer.copyBuffer(vk_rectStagingBuffer.buffer, vk_vertexBuffer, 1, &copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        return Result::eSuccess;
    }
}