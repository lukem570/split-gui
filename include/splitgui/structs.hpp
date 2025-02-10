#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.hpp"

#include <cinttypes>
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

        Vec4 operator+(Vec4 operand);
        Vec4 operator-(Vec4 operand);
        Vec4 operator*(Vec4 operand);
        Vec4 operator/(Vec4 operand);
    };

    struct IVec4 {
        union {int x; int r;};
        union {int y; int g;};
        union {int z; int b;};
        union {int w; int a;};

        IVec4 operator+(IVec4 operand);
        IVec4 operator-(IVec4 operand);
        IVec4 operator*(IVec4 operand);
        IVec4 operator/(IVec4 operand);
    };

    struct Vec3 {
        union {float x; float r;};
        union {float y; float g;};
        union {float z; float b;};

        Vec3 operator+(Vec3 operand);
        Vec3 operator-(Vec3 operand);
        Vec3 operator*(Vec3 operand);
        Vec3 operator/(Vec3 operand);
    };

    struct IVec3 {
        union {int x; int r;};
        union {int y; int g;};
        union {int z; int b;};

        IVec3 operator+(IVec3 operand);
        IVec3 operator-(IVec3 operand);
        IVec3 operator*(IVec3 operand);
        IVec3 operator/(IVec3 operand);
    };

    struct Vec2 {
        union {float x; float r;};
        union {float y; float g;};

        Vec2 operator+(Vec2 operand);
        Vec2 operator-(Vec2 operand);
        Vec2 operator*(Vec2 operand);
        Vec2 operator/(Vec2 operand);
    };

    struct IVec2 {
        union {int x; int r;};
        union {int y; int g;};

        IVec2 operator+(IVec2 operand);
        IVec2 operator-(IVec2 operand);
        IVec2 operator*(IVec2 operand);
        IVec2 operator/(IVec2 operand);
    };

    Vec4 Vec4::operator+(Vec4 operand) { return { (float)x + operand.x, (float)y + operand.y, (float)z + operand.z, (float)w + operand.w }; }
    Vec4 Vec4::operator-(Vec4 operand) { return { (float)x - operand.x, (float)y - operand.y, (float)z + operand.z, (float)w + operand.w }; }
    Vec4 Vec4::operator*(Vec4 operand) { return { (float)x * operand.x, (float)y * operand.y, (float)z + operand.z, (float)w + operand.w }; }
    Vec4 Vec4::operator/(Vec4 operand) { return { (float)x / operand.x, (float)y / operand.y, (float)z + operand.z, (float)w + operand.w }; }

    IVec4 IVec4::operator+(IVec4 operand) { return { (int)x + operand.x, (int)y + operand.y, (int)z + operand.z, (int)w + operand.w }; }
    IVec4 IVec4::operator-(IVec4 operand) { return { (int)x - operand.x, (int)y - operand.y, (int)z + operand.z, (int)w + operand.w }; }
    IVec4 IVec4::operator*(IVec4 operand) { return { (int)x * operand.x, (int)y * operand.y, (int)z + operand.z, (int)w + operand.w }; }
    IVec4 IVec4::operator/(IVec4 operand) { return { (int)x / operand.x, (int)y / operand.y, (int)z + operand.z, (int)w + operand.w }; }

    Vec3 Vec3::operator+(Vec3 operand) { return { (float)x + operand.x, (float)y + operand.y, (float)z + operand.z }; }
    Vec3 Vec3::operator-(Vec3 operand) { return { (float)x - operand.x, (float)y - operand.y, (float)z + operand.z }; }
    Vec3 Vec3::operator*(Vec3 operand) { return { (float)x * operand.x, (float)y * operand.y, (float)z + operand.z }; }
    Vec3 Vec3::operator/(Vec3 operand) { return { (float)x / operand.x, (float)y / operand.y, (float)z + operand.z }; }

    IVec3 IVec3::operator+(IVec3 operand) { return { (int)x + operand.x, (int)y + operand.y, (int)z + operand.z }; }
    IVec3 IVec3::operator-(IVec3 operand) { return { (int)x - operand.x, (int)y - operand.y, (int)z + operand.z }; }
    IVec3 IVec3::operator*(IVec3 operand) { return { (int)x * operand.x, (int)y * operand.y, (int)z + operand.z }; }
    IVec3 IVec3::operator/(IVec3 operand) { return { (int)x / operand.x, (int)y / operand.y, (int)z + operand.z }; }

    Vec2 Vec2::operator+(Vec2 operand) { return { (float)x + operand.x, (float)y + operand.y }; }
    Vec2 Vec2::operator-(Vec2 operand) { return { (float)x - operand.x, (float)y - operand.y }; }
    Vec2 Vec2::operator*(Vec2 operand) { return { (float)x * operand.x, (float)y * operand.y }; }
    Vec2 Vec2::operator/(Vec2 operand) { return { (float)x / operand.x, (float)y / operand.y }; }

    IVec2 IVec2::operator+(IVec2 operand) { return { (int)x + operand.x, (int)y + operand.y }; }
    IVec2 IVec2::operator-(IVec2 operand) { return { (int)x - operand.x, (int)y - operand.y }; }
    IVec2 IVec2::operator*(IVec2 operand) { return { (int)x * operand.x, (int)y * operand.y }; }
    IVec2 IVec2::operator/(IVec2 operand) { return { (int)x / operand.x, (int)y / operand.y }; }

    struct Mat2 {
        float a1, a2;
        float b1, b2;
    };

    struct Mat3 {
        alignas(16) Vec3 a;
        alignas(16) Vec3 b;
        alignas(16) Vec3 c;

        static Mat3 eulerRotationMatrix(Vec3 rotVec) {
            Mat3 rotation;

            float sinX = std::sin(rotVec.x);
            float cosX = std::cos(rotVec.x);

            float sinY = std::sin(rotVec.y);
            float cosY = std::cos(rotVec.y);

            float sinZ = std::sin(rotVec.z);
            float cosZ = std::cos(rotVec.z);

            rotation.a.x = cosY * cosZ;
            rotation.a.y = sinX * sinY * cosZ - cosX * sinZ;
            rotation.a.z = cosX * sinY * cosZ + sinX * sinZ;

            rotation.b.x = cosY * sinZ;
            rotation.b.y = sinX * sinY * sinZ + cosX * cosZ;
            rotation.b.z = cosX * sinY * sinZ - sinX * cosZ;

            rotation.c.x = -sinY;
            rotation.c.y = sinX * cosY;
            rotation.c.z = cosX * cosY;

            return rotation;
        }
    };

    struct Mat4 {
        float a1, a2, a3, a4;
        float b1, b2, b3, b4;
        float c1, c2, c3, c4;
        float d1, d2, d3, d4;
    };

    struct Quaternion {
        float x, y, z, w;

        Mat3 convertToMatrix();
        void rotate(Vec3 axis, float angle);
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

    struct Transform {
        Vec3 position = {0, 0, 0};
        Vec3 rotation = {0, 0, 0};
        Vec3 scale    = {1, 1, 1};
    };

    struct alignas(16) SceneObj {
        RectObj viewport;
        Mat3    cameraRotation;
        Vec3    cameraPosition;
    };

    struct Vertex {
        Vec3 pos;
        Vec3 color; // could be union
        Vec2 textureCord;
    };

    struct VertexBufferObject {
        Vertex   vertex;
        uint16_t flags;
        uint16_t sceneNumber;
        uint16_t textureNumber;
    };

    struct RectRef {
        int bottomLeft;
        int bottomRight;
        int topLeft;
        int topRight;
    };

    struct VertexUniformObject {
        IVec2 screenSize;
    };

    enum class UnitExpressionTokenType {
        eEndOfFile,
        eLiteral,
        eBeginBracket,
        eEndBracket,
        eUnit,
        eVector,
        eBinaryOp,
        eCall,
        eComma,
    };

    struct UnitExpressionToken {
        UnitExpressionTokenType type;
        std::string value;
    };

    struct UnitExpression {
        
        enum class Type {
            eLiteral,
            eVector,
            eCall,
            eBinaryOp,
        };

        enum class UnitType {
            ePercent,
            ePixel,
            eUnsigned,
        };

        enum class BinaryOpType {
            eAdd,
            eSubtract,
            eMultiply,
            eDivide,
        };

        struct Literal {
            UnitType type;
            double value;
        };

        struct Vector {
            bool isIVec;
            unsigned int size;
            UnitExpression** values;
        };

        struct Call {
            //std::string alias;
            //std::vector<UnitExpression*> params;
        };

        struct BinaryOp {
            UnitExpression* left;
            BinaryOpType    oper;
            UnitExpression* right;
        };

        union {
            Literal  literal;
            Vector   vector;
            Call     call;
            BinaryOp binary;
        };
        Type type;

        UnitExpression(Type type) : type(type) {}
        ~UnitExpression() = default;
    };

    struct UnitExpressionValue {
        enum class Type {
            eVector,
            eNumber
        };

        union {
            double number;
            struct {
                int  size;
                bool isInt;
                union {
                    IVec4 ivec4;
                    IVec3 ivec3;
                    IVec2 ivec2;
                    Vec4   vec4;
                    Vec3   vec3;
                    Vec2   vec2;
                };
            } vector;
        };
        Type type;

        UnitExpressionValue() = default;

        UnitExpressionValue(double number) : number(number), type(Type::eNumber) {} 

        UnitExpressionValue(IVec4 vec) : vector{.size = 4, .isInt = true,  .ivec4 = vec}, type(Type::eVector) {} 
        UnitExpressionValue( Vec4 vec) : vector{.size = 4, .isInt = false,  .vec4 = vec}, type(Type::eVector) {} 
        UnitExpressionValue(IVec3 vec) : vector{.size = 3, .isInt = true,  .ivec3 = vec}, type(Type::eVector) {} 
        UnitExpressionValue( Vec3 vec) : vector{.size = 3, .isInt = false,  .vec3 = vec}, type(Type::eVector) {} 
        UnitExpressionValue(IVec2 vec) : vector{.size = 2, .isInt = true,  .ivec2 = vec}, type(Type::eVector) {} 
        UnitExpressionValue( Vec2 vec) : vector{.size = 2, .isInt = false,  .vec2 = vec}, type(Type::eVector) {} 

        UnitExpressionValue operator+(const UnitExpressionValue& operand);
        UnitExpressionValue operator-(const UnitExpressionValue& operand);
        UnitExpressionValue operator*(const UnitExpressionValue& operand);
        UnitExpressionValue operator/(const UnitExpressionValue& operand);
    };

    struct XmlToken {
        XmlTokenType type;
        std::string value;
    };

#ifdef SPLIT_GUI_USE_VULKAN

    enum DescriporBindings {
        eSceneData     = 0,
        eGlyphs        = 1,
        eVertexUniform = 2,
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