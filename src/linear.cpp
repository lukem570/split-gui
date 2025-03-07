#include <splitgui/structs.hpp>

namespace SplitGui {
    Vec4 Vec4::operator+(Vec4 operand) { return { (float)x + operand.x, (float)y + operand.y, (float)z + operand.z, (float)w + operand.w }; }
    Vec4 Vec4::operator-(Vec4 operand) { return { (float)x - operand.x, (float)y - operand.y, (float)z - operand.z, (float)w - operand.w }; }
    Vec4 Vec4::operator*(Vec4 operand) { return { (float)x * operand.x, (float)y * operand.y, (float)z * operand.z, (float)w * operand.w }; }
    Vec4 Vec4::operator/(Vec4 operand) { return { (float)x / operand.x, (float)y / operand.y, (float)z / operand.z, (float)w / operand.w }; }

    IVec4 IVec4::operator+(IVec4 operand) { return { (int)x + operand.x, (int)y + operand.y, (int)z + operand.z, (int)w + operand.w }; }
    IVec4 IVec4::operator-(IVec4 operand) { return { (int)x - operand.x, (int)y - operand.y, (int)z - operand.z, (int)w - operand.w }; }
    IVec4 IVec4::operator*(IVec4 operand) { return { (int)x * operand.x, (int)y * operand.y, (int)z * operand.z, (int)w * operand.w }; }
    IVec4 IVec4::operator/(IVec4 operand) { return { (int)x / operand.x, (int)y / operand.y, (int)z / operand.z, (int)w / operand.w }; }

    Vec3 Vec3::operator+(Vec3 operand) { return { (float)x + operand.x, (float)y + operand.y, (float)z + operand.z }; }
    Vec3 Vec3::operator-(Vec3 operand) { return { (float)x - operand.x, (float)y - operand.y, (float)z - operand.z }; }
    Vec3 Vec3::operator*(Vec3 operand) { return { (float)x * operand.x, (float)y * operand.y, (float)z * operand.z }; }
    Vec3 Vec3::operator/(Vec3 operand) { return { (float)x / operand.x, (float)y / operand.y, (float)z / operand.z }; }

    IVec3 IVec3::operator+(IVec3 operand) { return { (int)x + operand.x, (int)y + operand.y, (int)z + operand.z }; }
    IVec3 IVec3::operator-(IVec3 operand) { return { (int)x - operand.x, (int)y - operand.y, (int)z - operand.z }; }
    IVec3 IVec3::operator*(IVec3 operand) { return { (int)x * operand.x, (int)y * operand.y, (int)z * operand.z }; }
    IVec3 IVec3::operator/(IVec3 operand) { return { (int)x / operand.x, (int)y / operand.y, (int)z / operand.z }; }

    Vec2 Vec2::operator+(Vec2 operand) { return { (float)x + operand.x, (float)y + operand.y }; }
    Vec2 Vec2::operator-(Vec2 operand) { return { (float)x - operand.x, (float)y - operand.y }; }
    Vec2 Vec2::operator*(Vec2 operand) { return { (float)x * operand.x, (float)y * operand.y }; }
    Vec2 Vec2::operator/(Vec2 operand) { return { (float)x / operand.x, (float)y / operand.y }; }

    IVec2 IVec2::operator+(IVec2 operand) { return { (int)x + operand.x, (int)y + operand.y }; }
    IVec2 IVec2::operator-(IVec2 operand) { return { (int)x - operand.x, (int)y - operand.y }; }
    IVec2 IVec2::operator*(IVec2 operand) { return { (int)x * operand.x, (int)y * operand.y }; }
    IVec2 IVec2::operator/(IVec2 operand) { return { (int)x / operand.x, (int)y / operand.y }; }

    bool Vec4::operator==(Vec4 operand) { return (x == operand.x && y == operand.y && z == operand.z && w == operand.w); }
    bool Vec4::operator!=(Vec4 operand) { return (x != operand.x && y != operand.y && z != operand.z && w != operand.w); }

    bool IVec4::operator==(IVec4 operand) { return (x == operand.x && y == operand.y && z == operand.z && w == operand.w); }
    bool IVec4::operator!=(IVec4 operand) { return (x != operand.x && y != operand.y && z != operand.z && w != operand.w); }

    bool Vec3::operator==(Vec3 operand) { return (x == operand.x && y == operand.y && z == operand.z); }
    bool Vec3::operator!=(Vec3 operand) { return (x != operand.x && y != operand.y && z != operand.z); }

    bool IVec3::operator==(IVec3 operand) { return (x == operand.x && y == operand.y && z == operand.z); }
    bool IVec3::operator!=(IVec3 operand) { return (x != operand.x && y != operand.y && z != operand.z); }

    bool Vec2::operator==(Vec2 operand) { return (x == operand.x && y == operand.y); }
    bool Vec2::operator!=(Vec2 operand) { return (x != operand.x && y != operand.y); }

    bool IVec2::operator==(IVec2 operand) { return (x == operand.x && y == operand.y); }
    bool IVec2::operator!=(IVec2 operand) { return (x != operand.x && y != operand.y); }

    float Vec4::dot(const Vec4& operand) { return x * operand.x + y * operand.y + z * operand.z + w * operand.w; }
    float IVec4::dot(const IVec4& operand) { return x * operand.x + y * operand.y + z * operand.z + w * operand.w; }
    float Vec3::dot(const Vec3& operand) { return x * operand.x + y * operand.y + z * operand.z; }
    float IVec3::dot(const IVec3& operand) { return x * operand.x + y * operand.y + z * operand.z; }
    float Vec2::dot(const Vec2& operand) { return x * operand.x + y * operand.y; }
    float IVec2::dot(const IVec2& operand) { return x * operand.x + y * operand.y; }

    void Vec4::normalize() {
        float len = std::sqrt(x*x + y*y + z*z + w*w);
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }

    void Vec3::normalize() {
        float len = std::sqrt(x*x + y*y + z*z);
        x /= len;
        y /= len;
        z /= len;
    }

    void Vec2::normalize() {
        float len = std::sqrt(x*x + y*y);
        x /= len;
        y /= len;
    }

    Mat4 Mat4::operator*(const Mat4& operand) {
        Mat4 mul;

        mul.a.x = a.x * operand.a.x + a.y * operand.b.x + a.z * operand.c.x + a.w * operand.d.x;
        mul.a.y = a.x * operand.a.y + a.y * operand.b.y + a.z * operand.c.y + a.w * operand.d.y;
        mul.a.z = a.x * operand.a.z + a.y * operand.b.z + a.z * operand.c.z + a.w * operand.d.z;
        mul.a.w = a.x * operand.a.w + a.y * operand.b.w + a.z * operand.c.w + a.w * operand.d.w;

        mul.b.x = b.x * operand.a.x + b.y * operand.b.x + b.z * operand.c.x + b.w * operand.d.x;
        mul.b.y = b.x * operand.a.y + b.y * operand.b.y + b.z * operand.c.y + b.w * operand.d.y;
        mul.b.z = b.x * operand.a.z + b.y * operand.b.z + b.z * operand.c.z + b.w * operand.d.z;
        mul.b.w = b.x * operand.a.w + b.y * operand.b.w + b.z * operand.c.w + b.w * operand.d.w;

        mul.c.x = c.x * operand.a.x + c.y * operand.b.x + c.z * operand.c.x + c.w * operand.d.x;
        mul.c.y = c.x * operand.a.y + c.y * operand.b.y + c.z * operand.c.y + c.w * operand.d.y;
        mul.c.z = c.x * operand.a.z + c.y * operand.b.z + c.z * operand.c.z + c.w * operand.d.z;
        mul.c.w = c.x * operand.a.w + c.y * operand.b.w + c.z * operand.c.w + c.w * operand.d.w;

        mul.d.x = d.x * operand.a.x + d.y * operand.b.x + d.z * operand.c.x + d.w * operand.d.x;
        mul.d.y = d.x * operand.a.y + d.y * operand.b.y + d.z * operand.c.y + d.w * operand.d.y;
        mul.d.z = d.x * operand.a.z + d.y * operand.b.z + d.z * operand.c.z + d.w * operand.d.z;
        mul.d.w = d.x * operand.a.w + d.y * operand.b.w + d.z * operand.c.w + d.w * operand.d.w;

        return mul;
    }

    Mat4 Mat4::operator+(const Mat4& operand) {
        Mat4 mul;

        mul.a.x = a.x + operand.a.x;
        mul.a.y = a.y + operand.a.y;
        mul.a.z = a.z + operand.a.z;
        mul.a.w = a.w + operand.a.w;

        mul.b.x = b.x + operand.b.x;
        mul.b.y = b.y + operand.b.y;
        mul.b.z = b.z + operand.b.z;
        mul.b.w = b.w + operand.b.w;

        mul.c.x = c.x + operand.c.x;
        mul.c.y = c.y + operand.c.y;
        mul.c.z = c.z + operand.c.z;
        mul.c.w = c.w + operand.c.w;

        mul.d.x = d.x + operand.d.x;
        mul.d.y = d.y + operand.d.y;
        mul.d.z = d.z + operand.d.z;
        mul.d.w = d.w + operand.d.w;

        return mul;
    }

    Mat4 Mat4::xRotationMatrix(float theta) {
        Mat4 rotation;

        float s = std::sin(theta);
        float c = std::cos(theta);

        rotation.a.x = 1;
        rotation.a.y = 0;
        rotation.a.z = 0;
        rotation.a.w = 0;

        rotation.b.x = 0;
        rotation.b.y = c;
        rotation.b.z = -s;
        rotation.b.w = 0;

        rotation.c.x = 0;
        rotation.c.y = s;
        rotation.c.z = c;
        rotation.c.w = 0;

        rotation.d.x = 0;
        rotation.d.y = 0;
        rotation.d.z = 0;
        rotation.d.w = 1;

        return rotation;
    }

    Mat4 Mat4::yRotationMatrix(float theta) {
        Mat4 rotation;

        float s = std::sin(theta);
        float c = std::cos(theta);

        rotation.a.x = c;
        rotation.a.y = 0;
        rotation.a.z = s;
        rotation.a.w = 0;

        rotation.b.x = 0;
        rotation.b.y = 1;
        rotation.b.z = 0;
        rotation.b.w = 0;

        rotation.c.x = -s;
        rotation.c.y = 0;
        rotation.c.z = c;
        rotation.c.w = 0;

        rotation.d.x = 0;
        rotation.d.y = 0;
        rotation.d.z = 0;
        rotation.d.w = 1;

        return rotation;
    }

    Mat4 Mat4::zRotationMatrix(float theta) {
        Mat4 rotation;

        float s = std::sin(theta);
        float c = std::cos(theta);

        rotation.a.x = c;
        rotation.a.y = -s;
        rotation.a.z = 0;
        rotation.a.w = 0;

        rotation.b.x = s;
        rotation.b.y = c;
        rotation.b.z = 0;
        rotation.b.w = 0;

        rotation.c.x = 0;
        rotation.c.y = 0;
        rotation.c.z = 1;
        rotation.c.w = 0;

        rotation.d.x = 0;
        rotation.d.y = 0;
        rotation.d.z = 0;
        rotation.d.w = 1;

        return rotation;
    }

    Mat4 Mat4::orthographicProjection(float far, float near) {
        Mat4 projection;

        projection.a.x = 1;
        projection.a.y = 0;
        projection.a.z = 0;
        projection.a.w = 0;

        projection.b.x = 0;
        projection.b.y = 1;
        projection.b.z = 0;
        projection.b.w = 0;

        projection.c.x = 0;
        projection.c.y = 0;
        projection.c.z = 2.0f / (far - near);
        projection.c.w = 0;

        projection.d.x = 0;
        projection.d.y = 0;
        projection.d.z = 0;
        projection.d.w = 1;

        return projection;
    }

    Mat4 Mat4::perspectiveProjection(float fieldOfView, float far, float near) {
        Mat4 projection;

        float fov = std::tan(fieldOfView / 2);

        projection.a.x = 1.0f / fov;
        projection.a.y = 0;
        projection.a.z = 0;
        projection.a.w = 0;

        projection.b.x = 0;
        projection.b.y = 1.0f / fov;
        projection.b.z = 0;
        projection.b.w = 0;

        projection.c.x = 0;
        projection.c.y = 0;
        projection.c.z = far / (near - far);
        projection.c.w = (near * far) / (near - far);

        projection.d.x = 0;
        projection.d.y = 0;
        projection.d.z = -1;
        projection.d.w = 0;

        return projection;
    }

    Mat4 Mat4::staticModel() {
        Mat4 model;

        model.a.x = 1;
        model.a.y = 0;
        model.a.z = 0;
        model.a.w = 0;

        model.b.x = 0;
        model.b.y = 1;
        model.b.z = 0;
        model.b.w = 0;

        model.c.x = 0;
        model.c.y = 0;
        model.c.z = 1;
        model.c.w = 0;

        model.d.x = 0;
        model.d.y = 0;
        model.d.z = 0;
        model.d.w = 1;

        return model;
    }

    bool RectObj::inside(IVec2 point) {
        return x <= point.x && y <= point.y && x + width >= point.x && y + height >= point.y;
    }

    bool RectObj::atLeftEdge(IVec2 point, int edgeWidth) {
        return x <= point.x && x + edgeWidth >= point.x;
    }

    bool RectObj::atRightEdge(IVec2 point, int edgeWidth) {
        return x + width - edgeWidth <= point.x && x + width >= point.x;
    }

    bool RectObj::atTopEdge(IVec2 point, int edgeWidth) {
        return y <= point.y && y + edgeWidth >= point.y;
    }

    bool RectObj::atBottomEdge(IVec2 point, int edgeWidth) {
        return y + height - edgeWidth <= point.y && y + height >= point.y;
    }

    bool RectObj::atEdge(IVec2 point, int edgeWidth) {
        return atLeftEdge(point, edgeWidth) || atRightEdge(point, edgeWidth) || atTopEdge(point, edgeWidth) || atBottomEdge(point, edgeWidth);
    }
}