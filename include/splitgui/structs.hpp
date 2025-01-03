#include <array>
#include <splitgui/lib.h>

namespace SplitGui {
    struct Vec3 {
        float x;
        float y;
        float z;
    };

    struct IVec3 {
        int x;
        int y;
        int z;
    };

    struct Vec2 {
        float x;
        float y;
    };

    struct IVec2 {
        float x;
        float y;
    };

    struct Vertex {
        Vec2 pos;
        Vec3 color;

        static vk::VertexInputBindingDescription getBindingDescription() {
            vk::VertexInputBindingDescription bindingDescription;
            bindingDescription.binding   = 0;
            bindingDescription.stride    = sizeof(Vertex);
            bindingDescription.inputRate = vk::VertexInputRate::eVertex;

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;
            attributeDescriptions[0].binding  = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format   = vk::Format::eR32G32Sfloat;
            attributeDescriptions[0].offset   = offsetof(Vertex, pos);

            attributeDescriptions[1].binding  = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format   = vk::Format::eR32G32Sfloat;
            attributeDescriptions[1].offset   = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };
}