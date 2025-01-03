#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.h"

#include <array>

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
        float x;
        float y;
    };

    struct Vertex {
        Vec2 pos;
        Vec3 color;
    };
}

#endif