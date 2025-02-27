#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.hpp"

#include <cinttypes>
#include <array>
#include <vector>
#include <string>
#include <variant>

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
        eWorldSpace  = 0x10,
    };

    struct SPLITGUI_EXPORT Vec4 {
        union {float x; float r;};
        union {float y; float g;};
        union {float z; float b;};
        union {float w; float a;};

        Vec4 operator+(Vec4 operand);
        Vec4 operator-(Vec4 operand);
        Vec4 operator*(Vec4 operand);
        Vec4 operator/(Vec4 operand);

        bool operator==(Vec4 operand);
        bool operator!=(Vec4 operand);

        float dot(const Vec4& operand);
        void normalize();
    };

    struct SPLITGUI_EXPORT IVec4 {
        union {int x; int r;};
        union {int y; int g;};
        union {int z; int b;};
        union {int w; int a;};

        IVec4 operator+(IVec4 operand);
        IVec4 operator-(IVec4 operand);
        IVec4 operator*(IVec4 operand);
        IVec4 operator/(IVec4 operand);

        bool operator==(IVec4 operand);
        bool operator!=(IVec4 operand);

        float dot(const IVec4& operand);
    };

    struct SPLITGUI_EXPORT Vec3 {
        union {float x; float r;};
        union {float y; float g;};
        union {float z; float b;};

        Vec3 operator+(Vec3 operand);
        Vec3 operator-(Vec3 operand);
        Vec3 operator*(Vec3 operand);
        Vec3 operator/(Vec3 operand);

        bool operator==(Vec3 operand);
        bool operator!=(Vec3 operand);

        Vec3 cross(const Vec3& operand) const {
            return Vec3{
                y * operand.z - z * operand.y,
                z * operand.x - x * operand.z,
                x * operand.y - y * operand.x,
            };
        }

        float dot(const Vec3& operand);
        void normalize();
    };

    struct SPLITGUI_EXPORT IVec3 {
        union {int x; int r;};
        union {int y; int g;};
        union {int z; int b;};

        IVec3 operator+(IVec3 operand);
        IVec3 operator-(IVec3 operand);
        IVec3 operator*(IVec3 operand);
        IVec3 operator/(IVec3 operand);

        bool operator==(IVec3 operand);
        bool operator!=(IVec3 operand);

        float dot(const IVec3& operand);
    };

    struct SPLITGUI_EXPORT Vec2 {
        union {float x; float r;};
        union {float y; float g;};

        Vec2 operator+(Vec2 operand);
        Vec2 operator-(Vec2 operand);
        Vec2 operator*(Vec2 operand);
        Vec2 operator/(Vec2 operand);

        bool operator==(Vec2 operand);
        bool operator!=(Vec2 operand);

        float dot(const Vec2& operand);
        void normalize();
    };

    struct SPLITGUI_EXPORT IVec2 {
        union {int x; int r;};
        union {int y; int g;};

        IVec2 operator+(IVec2 operand);
        IVec2 operator-(IVec2 operand);
        IVec2 operator*(IVec2 operand);
        IVec2 operator/(IVec2 operand);

        bool operator==(IVec2 operand);
        bool operator!=(IVec2 operand);

        float dot(const IVec2& operand);
    };

    struct SPLITGUI_EXPORT Mat2 {
        alignas(8) Vec2 a;
        alignas(8) Vec2 b;
    };

    struct SPLITGUI_EXPORT Mat3 {
        alignas(16) Vec3 a;
        alignas(16) Vec3 b;
        alignas(16) Vec3 c;
    };

    struct SPLITGUI_EXPORT Mat4 {
        alignas(16) Vec4 a;
        alignas(16) Vec4 b;
        alignas(16) Vec4 c;
        alignas(16) Vec4 d;

        Mat4 operator*(const Mat4& operand);
        Mat4 operator+(const Mat4& operand);

        static Mat4 xRotationMatrix(float theta);
        static Mat4 yRotationMatrix(float theta);
        static Mat4 zRotationMatrix(float theta);

        static Mat4 orthographicProjection(float far = 100.0f, float near = 0.1f);
        static Mat4 perspectiveProjection(float fieldOfView, float far = 100.0f, float near = 0.1f);
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
        
        bool inside(IVec2);
        bool atEdge(IVec2, int);
        bool atLeftEdge(IVec2, int);
        bool atRightEdge(IVec2, int);
        bool atTopEdge(IVec2, int);
        bool atBottomEdge(IVec2, int);
    };

    struct Transform {
        Vec3 position = {0, 0, 0};
        Vec3 rotation = {0, 0, 0};
        Vec3 scale    = {1, 1, 1};
    };

    struct alignas(16) SceneObj {
        RectObj viewport;
        Mat4    cameraView;
        Mat4    cameraProjection;
        Vec3    cameraPosition;
    };

    struct Vertex {
        Vec3 pos;
        Vec3 color; // TODO: make material object
        Vec2 textureCord;
    };

    struct VertexBufferObject {
        Vertex   vertex;
        uint16_t flags;
        uint16_t sceneNumber;
        uint16_t textureNumber;
        Vec3     normal;
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

    struct MoveTo {
        Vec2 from;
    };

    struct LinearContour {
        Vec2 to;
    };

    struct QuadraticBezierContour {
        Vec2 control;
        Vec2 to;
    };

    struct CubicBezierContour {
        Vec2 controlA;
        Vec2 controlB;
        Vec2 to;
    };

    typedef std::variant<MoveTo, LinearContour, QuadraticBezierContour, CubicBezierContour> Contour;

#ifdef SPLIT_GUI_USE_VULKAN

    enum DescriporBindings {
        eSceneData     = 0,
        eGlyphs        = 1,
        eVertexUniform = 2,
    };

    enum VertexLayout {
        eVertexPos         = 0,
        eVertexColor       = 1,
        eVertexTextureCord = 2,
        eFlags             = 3,
        eSceneNumber       = 4,
        eTextureNumber     = 5,
        eNormal            = 6,
    };

    struct MSDFImage {

    };

#endif
}

#endif