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

    void VulkanInterface::updateRect(RectRef& ref, Vec2 x1, Vec2 x2) {

        vertices[ref.bottomLeft].vertex.pos  = {std::min(x1.x, x2.x), std::min(x1.y, x2.y), 0};
        vertices[ref.bottomRight].vertex.pos = {std::max(x1.x, x2.x), std::min(x1.y, x2.y), 0};
        vertices[ref.topLeft].vertex.pos     = {std::min(x1.x, x2.x), std::max(x1.y, x2.y), 0};
        vertices[ref.topRight].vertex.pos    = {std::max(x1.x, x2.x), std::max(x1.y, x2.y), 0};

        markVerticesForUpdate = true;

        SPLITGUI_LOG("Updated Rect: %d", ref.bottomLeft);
    }
}