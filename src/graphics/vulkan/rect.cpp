#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::drawRect(Vec2 x1, Vec2 x2, Vec3 color) {

        int verticesOffset = vertices.size();

        indices.push_back(verticesOffset + 2);
        indices.push_back(verticesOffset + 0);
        indices.push_back(verticesOffset + 3);
        indices.push_back(verticesOffset + 3);
        indices.push_back(verticesOffset + 0);
        indices.push_back(verticesOffset + 1);
        
        Vertex bottomLeft;
        bottomLeft.pos   = {std::min(x1.x, x2.x), std::min(x1.y, x2.y)};
        bottomLeft.color = color;

        Vertex bottomRight;
        bottomRight.pos   = {std::max(x1.x, x2.x), std::min(x1.y, x2.y)};
        bottomRight.color = color;

        Vertex topLeft;
        topLeft.pos   = {std::min(x1.x, x2.x), std::max(x1.y, x2.y)};
        topLeft.color = color;

        Vertex topRight;
        topRight.pos   = {std::max(x1.x, x2.x), std::max(x1.y, x2.y)};
        topRight.color = color;

        vertices.push_back({bottomLeft,  (uint16_t)VertexFlags::eNone, 0, 0});
        vertices.push_back({bottomRight, (uint16_t)VertexFlags::eNone, 0, 0});
        vertices.push_back({topLeft,     (uint16_t)VertexFlags::eNone, 0, 0});
        vertices.push_back({topRight,    (uint16_t)VertexFlags::eNone, 0, 0});
    }
}