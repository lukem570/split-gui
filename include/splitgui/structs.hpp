#ifndef SPLITGUI_STRUCTS_HPP
#define SPLITGUI_STRUCTS_HPP

#include "lib.hpp"

#include <cinttypes>
#include <array>
#include <vector>
#include <string>
#include <variant>
#include <mutex>
#include <condition_variable>
#include <list>
#include <optional>
#include <queue>

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

        std::string stringify();
    };

    struct SPLITGUI_EXPORT Vec3 {
        union {float x; float r;};
        union {float y; float g;};
        union {float z; float b;};

        Vec3() = default;
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
        Vec3(float f) : x(f), y(f), z(f){}

        Vec3 operator+(Vec3 operand);
        Vec3 operator-(Vec3 operand);
        Vec3 operator*(Vec3 operand);
        Vec3 operator/(Vec3 operand);

        bool operator==(Vec3 operand);
        bool operator!=(Vec3 operand);

        Vec3 cross(const Vec3& operand) const {
            return Vec3 {
                y * operand.z - z * operand.y,
                z * operand.x - x * operand.z,
                x * operand.y - y * operand.x,
            };
        }

        float dot(const Vec3& operand);
        void normalize();

        Vec4 extend(float value);
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

        std::string stringify();
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

        std::string stringify();
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
        union {
            struct {
                alignas(16) Vec4 a;
                alignas(16) Vec4 b;
                alignas(16) Vec4 c;
                alignas(16) Vec4 d;
            };
            float matrix[4][4];
        };

        Mat4 operator*(const Mat4& operand);
        Mat4 operator+(const Mat4& operand);

        Mat4 inverse();

        Vec4 operator*(const Vec4& operand);

        void updatePerspective(float fieldOfView, RectObj extent);

        static Mat4 xRotationMatrix(float theta);
        static Mat4 yRotationMatrix(float theta);
        static Mat4 zRotationMatrix(float theta);

        static Mat4 quaternionMatrix(Vec4 quaternion);

        static Mat4 orthographicProjection(float far = 100.0f, float near = 0.1f);
        static Mat4 perspectiveProjection(float fieldOfView, RectObj extent, float far = 100.0f, float near = 0.1f);
        static Mat4 ident();
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

        HexColor dim() {
            return IVec3{
                (int)(r * 0.80),
                (int)(g * 0.80),
                (int)(b * 0.80),
            };
        }

        HexColor dark() {
            return IVec3{
                (int)(r * 0.50),
                (int)(g * 0.50),
                (int)(b * 0.50),
            };
        }

        uint8_t b;
        uint8_t g; 
        uint8_t r;
    };

    template <typename T>
    struct LinkElement {
        T data;
        LinkElement<T>* next = nullptr;
        LinkElement<T>* previous = nullptr;
    };

    template <typename T>
    class LinkList {
        public:
            ~LinkList();

            unsigned int size();
            void erase(LinkElement<T>* start, LinkElement<T>* end);
            void clear();
            std::optional<unsigned int> offset(LinkElement<T>* element);
            LinkElement<T>* push(T data);
            T* array();
            T* sliceArray(LinkElement<T>* start, LinkElement<T>* end);
        
        private:
            unsigned int listSize = 0;
            LinkElement<T>* first = nullptr;
            LinkElement<T>* last = nullptr;
            T* arrayPtr = nullptr;
    };

    struct Transform {
        Vec3 position = {0, 0, 0};
        Vec3 rotation = {0, 0, 0};
        Vec3 scale    = {1, 1, 1};
    };

    struct alignas(16) SceneObj {
        Mat4    cameraView;
        Mat4    cameraProjection;
        Vec3    cameraPosition;
    };

    struct Vertex {
        Vec3 pos;
        Vec3 color; // TODO: make material object
        float opacity = 1.0f;
        Vec2 textureCord;
    };

    struct VertexBufferObject {
        Vertex   vertex;
        uint16_t flags;
        uint16_t index; // stores scene number and texture number
    };

    struct SceneVertexBufferObject {
        Vertex   vertex;
        uint16_t flags;
        uint16_t textureNumber;
        uint16_t modelNumber;
        Vec3     normal;
    };

    struct alignas(16) VectorEdgeBufferObject {
        alignas(16) Vec3 start;
        alignas(16) Vec3 end;
    };

    struct RectRef {

        union {
            LinkElement<VertexBufferObject>* bottomLeft;
            LinkElement<VertexBufferObject>* verticesStart;
        };

        union {
            LinkElement<VertexBufferObject>* topRight;
            LinkElement<VertexBufferObject>* verticesEnd;
        };

        LinkElement<VertexBufferObject>* bottomRight = nullptr;
        LinkElement<VertexBufferObject>* topLeft     = nullptr;

        LinkElement<uint16_t>* indicesStart = nullptr;
        LinkElement<uint16_t>* indicesEnd   = nullptr;
    };

    struct TextureRef {
        unsigned int textureNumber;
    };

    struct SceneRef {
        unsigned int sceneNumber;
        RectRef      rect;
    };

    struct ModelRef {
        unsigned int modelNumber;
    };

    struct TextRef {
        std::vector<RectRef> rects;
        std::string          text;
    };

    struct VectorEngineRef {
        unsigned int instanceNumber;
    };

    struct EdgeRef {
        LinkElement<VectorEdgeBufferObject>* edgesStart = nullptr;
        LinkElement<VectorEdgeBufferObject>* edgesEnd   = nullptr;
    };

    struct TriangleBlock {
        LinkElement<uint16_t>* indicesStart = nullptr;
        LinkElement<uint16_t>* indicesEnd   = nullptr;
        LinkElement<SceneVertexBufferObject>* verticesStart = nullptr;
        LinkElement<SceneVertexBufferObject>* verticesEnd   = nullptr;
    };

    struct TriangleRef {
        std::list<TriangleBlock> triangleBlocks;

        void operator+=(TriangleRef& ref) {
            triangleBlocks.splice(triangleBlocks.end(), ref.triangleBlocks);
        }
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

    class FairMutex {
        public:
            void lock() {
                SPLITGUI_PROFILE;

                unsigned int myTicket;
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    myTicket = nextTicket++;
                }
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this, myTicket] { return myTicket == nowServing; });
            }
        
            void unlock() {
                SPLITGUI_PROFILE;

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    nowServing++;
                }
        
                cv.notify_all();
            }

            bool isLocked() {
                SPLITGUI_PROFILE;

                std::lock_guard<std::mutex> lock(mtx);
                return nextTicket != nowServing;
            }

        private:
            unsigned int nextTicket = 0;
            unsigned int nowServing = 0;
            std::mutex mtx;
            std::condition_variable cv;
        
    };

    template <typename T>
    class ThreadBuffer {
        public:

            void push(const T& data) {
                SPLITGUI_PROFILE;

                mtx.lock();

                availableIndices.push(resources.size());
                resources.push_back(std::move(data));

                mtx.unlock();
            }

            unsigned int acquireAvailable() {
                SPLITGUI_PROFILE;

                mtx.lock();

                std::unique_lock<std::mutex> lock(aq);

                cv.wait(lock, [this]() { return !availableIndices.empty(); });
                
                unsigned int index = availableIndices.front();
                availableIndices.pop();

                mtx.unlock();
                
                return index;
            }

            T& getData(unsigned int id) {
                SPLITGUI_PROFILE;

                return resources[id];
            }

            void release(unsigned int id) {
                SPLITGUI_PROFILE;

                mtx.lock();

                availableIndices.push(id);

                mtx.unlock();

                cv.notify_one();
            }

        private:
            std::vector<T> resources;
            std::queue<unsigned int> availableIndices;
            std::condition_variable cv;
            std::mutex aq;
            FairMutex mtx;
    };

    template<typename T>
    class ThreadLocal {
    public:
        T& get() {
            thread_local T value{};
            return value;
        }

        void operator=(const T& val) {
            get() = val;
        }

        operator T() const {
            return const_cast<ThreadLocal*>(this)->get();
        }
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

    struct LinearEdge {
        Vec3 from;
        Vec3 to;
    };

    struct QuadraticEdge {
        Vec3 from;
        Vec3 control;
        Vec3 to;
    };

    struct CubicEdge {
        Vec3 from;
        Vec3 control1;
        Vec3 control2;
        Vec3 to;
    };

    typedef std::variant<LinearEdge, QuadraticEdge, CubicEdge> Edge;

#ifdef SPLIT_GUI_USE_VULKAN

    struct DescriporBindings {
        enum {
            eGlyphs  = 0,
            eScenes  = 1,
            eTexture = 2,
        };
    };

    struct SceneDescriporBindings {
        enum {
            eSceneData = 0,
            eModels    = 1,
            eTexture   = 2,
        };
    };

    struct VectorEngineDescriporBindings {
        enum {
            eSceneData   = 0,
            eEdgesIn     = 1,
            eEdgesOut    = 2,
            eOutputImage = 3,
        };
    };

    struct VertexLayout {
        enum {
            eVertexPos         = 0,
            eVertexColor       = 1,
            eVertexTextureCord = 2,
            eFlags             = 3,
            eIndex             = 4,
        };
    };

    struct SceneVertexLayout {
        enum {
            eVertexPos         = 0,
            eVertexColor       = 1,
            eVertexTextureCord = 2,
            eFlags             = 3,
            eTextureNumber     = 4,
            eModelNumber       = 5,
            eNormal            = 6,
        };
    };

#endif
}

#endif