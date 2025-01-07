#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.h"

#include <array>
#include <vector>

namespace SplitGui {
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

    struct Mask { // RGBA mask 
        IVec2 size;
        std::vector<std::vector<Vec4>> pixels;
    };

    struct Vertex {
        Vec2 pos;
        Vec3 color;
    };
}

#endif