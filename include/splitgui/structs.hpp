#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.h"

#include <array>
#include <vector>
#include <string>

namespace SplitGui {

    enum class UnitType {
        ePixels,
        ePercent,
    };

    enum class UnitOperationType {
        eAdd,
        eSub,
        eMul,
        eDiv,
        eSame,
    };

    enum class VerticalAnchor {
        eTop,
        eCenter,
        eBottom,
    };

    enum class HorizontalAnchor {
        eLeft,
        eCenter,
        eRight,
    };

    struct Vec4 {
        float x;
        float y;
        float z;
        float w;
    };

    struct IVec4 {
        int x;
        int y;
        int z;
        int w;
    };

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
        int x;
        int y;
    };

    struct HexColor {
        HexColor() {}

        HexColor(int hex) {
            r = (hex >> (UINT8_WIDTH * 2)) & UINT8_MAX;
            g = (hex >> (UINT8_WIDTH * 1)) & UINT8_MAX;
            b = (hex >> (UINT8_WIDTH * 0)) & UINT8_MAX;
        }

        Vec3 normalize() {
            return {
                (float) r / (float) UINT8_MAX, 
                (float) g / (float) UINT8_MAX, 
                (float) b / (float) UINT8_MAX
            };
        }

        uint8_t b;
        uint8_t g; 
        uint8_t r;
    };

    struct RectObj {
        int width  = 0;
        int height = 0;
        int x      = 0;
        int y      = 0;
    };

    struct Vertex {
        Vec2 pos;
        Vec3 color;
    };

    struct Triangle {
        uint32_t classNumber = 0; // 0 = ui / root; >0 = scene number
    };

    struct Unit {
        UnitType type;
        std::string value;
    };

    struct UnitOperation {
        std::array<Unit, 2> operands;
        UnitOperationType type;
    };

#ifdef SPLIT_GUI_USE_VULKAN

    struct MSDFImage {
        vk::Image image;
        vk::ImageView imageView;
        //vk::Sampler sampler;
        vk::DeviceMemory imageMemory;
    };

#endif
}

#endif