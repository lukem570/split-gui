#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.h"

#include <array>
#include <vector>
#include <string>

namespace SplitGui {

    typedef uint16_t VertexFlags;

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

    enum class XmlTokenType {
        eTagOpen,
        eTagClose,
        eText,
        eAttribute,
        eEndOfFile,
        eError
    };

    enum VertexFlagsBits {
        eNone        = 0x00,
        eUseTexture  = 0x01,
        eTextureMsdf = 0x02,
        eTransparent = 0x04,
        eScene       = 0x08,
    };

    struct Vec4 {
        union {float x; float r;};
        union {float y; float g;};
        union {float z; float b;};
        union {float w; float a;};
    };

    struct IVec4 {
        union {int x; int r;};
        union {int y; int g;};
        union {int z; int b;};
        union {int w; int a;};
    };

    struct Vec3 {
        union {float x; float r;};
        union {float y; float g;};
        union {float z; float b;};
    };

    struct IVec3 {
        union {int x; int r;};
        union {int y; int g;};
        union {int z; int b;};
    };

    struct Vec2 {
        union {float x; float r;};
        union {float y; float g;};

        Vec2 operator+(const Vec2& operand) const {
            return {operand.x + x, operand.y + y};
        }
    };

    struct IVec2 {
        union {int x; int r;};
        union {int y; int g;};

        IVec2 operator+(IVec2 operand) { return { x + operand.x, y + operand.y }; }
        IVec2 operator-(IVec2 operand) { return { x - operand.x, y - operand.y }; }
        IVec2 operator*(IVec2 operand) { return { x * operand.x, y * operand.y }; }
        IVec2 operator/(IVec2 operand) { return { x / operand.x, y / operand.y }; }

        Vec2 operator+(Vec2 operand) { return { (float)x + operand.x, (float)y + operand.y }; }
        Vec2 operator-(Vec2 operand) { return { (float)x - operand.x, (float)y - operand.y }; }
        Vec2 operator*(Vec2 operand) { return { (float)x * operand.x, (float)y * operand.y }; }
        Vec2 operator/(Vec2 operand) { return { (float)x / operand.x, (float)y / operand.y }; }
        
    };

    struct HexColor {
        HexColor() {}

        HexColor(int hex) {
            r = (hex >> (UINT8_WIDTH * 2)) & UINT8_MAX;
            g = (hex >> (UINT8_WIDTH * 1)) & UINT8_MAX;
            b = (hex >> (UINT8_WIDTH * 0)) & UINT8_MAX;
        }

        HexColor(IVec3 hex) {
            r = hex.r;
            g = hex.g;
            b = hex.b;
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

        RectObj() {
            width = 0;
            height = 0;
            x = 0;
            y = 0;
        }

        union {
            IVec2 size;
            struct {
                int width;
                int height;
            };
        };

        union {
            IVec2 pos;
            struct {
                int x;
                int y;
            };
        };
        
    };

    struct SceneObj {
        RectObj  viewport;
        float    cameraFieldOfView;
        Vec3     cameraPosition;
        Vec3     cameraRotation;
    };

    struct Vertex {
        Vec2 pos;
        Vec3 color; // could be union
        Vec2 textureCord;
    };

    struct VertexBufferObject {
        Vertex   vertex;
        uint16_t flags;
        uint16_t sceneNumber;
        uint16_t textureNumber;
    };

    struct VertexUniformObject {
        IVec2    screenSize;
        uint16_t numScenes;
        uint16_t numTextures;
    };

    struct Unit {
        UnitType   type;
        std::string value;
    };

    struct UnitOperation {
        std::array<Unit, 2> operands;
        UnitOperationType   type;
    };

    struct XmlToken {
        XmlTokenType type;
        std::string value;
    };

#ifdef SPLIT_GUI_USE_VULKAN

    enum DescriporBindings {
        eVertexUniform  = 0,
        eSceneData      = 1,
        eGlyphs         = 2,
    };

    enum VertexLayout {
        eVertexPos = 0,
        eVertexColor = 1,
        eVertexTextureCord = 2,
        eFlags = 3,
        eSceneNumber = 4,
        eTextureNumber = 5,
    };

    struct MSDFImage {

    };

#endif
}

#endif