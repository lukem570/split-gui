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

    struct Rect {
        int width;
        int height;
        int x;
        int y;
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
}

#endif