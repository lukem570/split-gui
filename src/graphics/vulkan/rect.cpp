#include "vulkan.hpp"

namespace SplitGui {
    RectRef VulkanInterface::drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth, VertexFlags flags, uint16_t textureIndex) {

        int verticesOffset = vertices.size();

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

        int oldSize = vertices.size();

        vertices.resize(oldSize + 4);

        vertices[oldSize + 0] = {bottomLeft,  flags, 0, textureIndex};
        vertices[oldSize + 1] = {bottomRight, flags, 0, textureIndex};
        vertices[oldSize + 2] = {topLeft,     flags, 0, textureIndex};
        vertices[oldSize + 3] = {topRight,    flags, 0, textureIndex};

        RectRef refRet;
        refRet.bottomLeft  = oldSize + 0;
        refRet.bottomRight = oldSize + 1;
        refRet.topLeft     = oldSize + 2;
        refRet.topRight    = oldSize + 3;

        SPLITGUI_LOG("Created Rect: %d", refRet.bottomLeft);

        return refRet;
    }

    void VulkanInterface::updateRect(RectRef& ref, Vec2 x1, Vec2 x2, Vec3 color, float depth) {

        vertices[ref.bottomLeft].vertex.pos  = {std::min(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        vertices[ref.bottomRight].vertex.pos = {std::max(x1.x, x2.x), std::min(x1.y, x2.y), depth};
        vertices[ref.topLeft].vertex.pos     = {std::min(x1.x, x2.x), std::max(x1.y, x2.y), depth};
        vertices[ref.topRight].vertex.pos    = {std::max(x1.x, x2.x), std::max(x1.y, x2.y), depth};

        vertices[ref.bottomLeft].vertex.color  = color;
        vertices[ref.bottomRight].vertex.color = color;
        vertices[ref.topLeft].vertex.color     = color;
        vertices[ref.topRight].vertex.color    = color;

        markVerticesForUpdate = true;

        SPLITGUI_LOG("Updated Rect: %d", ref.bottomLeft);
    }

    Result VulkanInterface::submitRect(RectRef& ref) {

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

        std::memcpy(memory, vertices.data() + sizeof(VertexBufferObject) * ref.bottomLeft, rectSize);

        vk_device.unmapMemory(stagingBufferMemory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;
        copyRegion.size      = rectSize;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = sizeof(VertexBufferObject) * ref.bottomLeft;

        commandBuffer.copyBuffer(stagingBuffer, vk_vertexBuffer, 1, &copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        SPLITGUI_LOG("Submitted Rect");

        return Result::eSuccess;
    }
}