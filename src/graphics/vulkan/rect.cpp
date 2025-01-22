#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::drawRect(Vec2 x1, Vec2 x2, Vec3 color, VertexFlags flags, uint16_t textureIndex) {

        int verticesOffset = vertices.size();

        indices.push_back(verticesOffset + 2);
        indices.push_back(verticesOffset + 0);
        indices.push_back(verticesOffset + 3);
        indices.push_back(verticesOffset + 3);
        indices.push_back(verticesOffset + 0);
        indices.push_back(verticesOffset + 1);
        
        Vertex bottomLeft;
        bottomLeft.pos         = {std::min(x1.x, x2.x), std::min(x1.y, x2.y)};
        bottomLeft.color       = color;
        bottomLeft.textureCord = {0.0f, 0.0f};

        Vertex bottomRight;
        bottomRight.pos         = {std::max(x1.x, x2.x), std::min(x1.y, x2.y)};
        bottomRight.color       = color;
        bottomRight.textureCord = {1.0f, 0.0f};

        Vertex topLeft;
        topLeft.pos         = {std::min(x1.x, x2.x), std::max(x1.y, x2.y)};
        topLeft.color       = color;
        topLeft.textureCord = {0.0f, 1.0f};

        Vertex topRight;
        topRight.pos         = {std::max(x1.x, x2.x), std::max(x1.y, x2.y)};
        topRight.color       = color;
        topRight.textureCord = {1.0f, 1.0f};

        vertices.push_back({bottomLeft,  flags, 0, textureIndex});
        vertices.push_back({bottomRight, flags, 0, textureIndex});
        vertices.push_back({topLeft,     flags, 0, textureIndex});
        vertices.push_back({topRight,    flags, 0, textureIndex});
    }
}