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

    Vec4 Vec3::extend(float value) { return {x, y, z, value}; }

    void Vec4::normalize() {
        SPLITGUI_PROFILE;

        float len = std::sqrt(x*x + y*y + z*z + w*w);
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }

    void Vec3::normalize() {
        SPLITGUI_PROFILE;

        float len = std::sqrt(x*x + y*y + z*z);
        x /= len;
        y /= len;
        z /= len;
    }

    void Vec2::normalize() {
        SPLITGUI_PROFILE;

        float len = std::sqrt(x*x + y*y);
        x /= len;
        y /= len;
    }

    std::string IVec4::stringify() {
        SPLITGUI_PROFILE;

        std::stringstream str;

        str << "ivec4(";
        str << x << "u, ";
        str << y << "u, ";
        str << z << "u, ";
        str << w << "u)";

        return str.str();
    }

    std::string IVec3::stringify() {
        SPLITGUI_PROFILE;

        std::stringstream str;

        str << "ivec3(";
        str << x << "u, ";
        str << y << "u, ";
        str << z << "u)";

        return str.str();
    }

    std::string IVec2::stringify() {
        SPLITGUI_PROFILE;

        std::stringstream str;

        str << "ivec2(";
        str << x << "u, ";
        str << y << "u)";

        return str.str();
    }

    float determinant(Mat4& mat, int n) {
        float det = 0;
        if (n == 1) {
            return mat.matrix[0][0];
        }
    
        Mat4 temp;
        int sign = 1;
    
        for (int f = 0; f < n; f++) {
            int i = 0;
            for (int row = 1; row < n; row++) {
                int j = 0;
                for (int col = 0; col < n; col++) {
                    if (col != f) {
                        temp.matrix[i][j++] = mat.matrix[row][col];
                    }
                }
                i++;
            }
            det += sign * mat.matrix[0][f] * determinant(temp, n - 1);
            sign = -sign;
        }
    
        return det;
    }

    void getCofactor(Mat4& mat, Mat4& temp, int p, int q, int n) {
        int i = 0, j = 0;
    
        for (int row = 0; row < n; row++) {
            for (int col = 0; col < n; col++) {
                if (row != p && col != q) {
                    temp.matrix[i][j++] = mat.matrix[row][col];
                    if (j == n - 1) {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    void adjoint(Mat4& mat, Mat4& adj) {
        if (4 == 1) {
            adj.matrix[0][0] = 1;
            return;
        }
    
        int sign = 1;
        Mat4 temp;
    
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                getCofactor(mat, temp, i, j, 4);
                sign = ((i + j) % 2 == 0) ? 1 : -1;
                adj.matrix[j][i] = sign * determinant(temp, 3);
            }
        }
    }

    Mat4 Mat4::inverse() {
        float det = determinant(*this, 4);
        if (det == 0) {
            return {};
        }

        Mat4 adj;
        Mat4 ret;
        adjoint(*this, adj);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                ret.matrix[i][j] = adj.matrix[i][j] / det;
            }
        }

        return ret;
    }

    Vec4 Mat4::operator*(const Vec4& operand) {
        SPLITGUI_PROFILE;

        Vec4 vec;

        vec.x = operand.x * a.x + operand.y * a.y + operand.z * a.z + operand.w * a.w;
        vec.y = operand.x * b.x + operand.y * b.y + operand.z * b.z + operand.w * b.w;
        vec.z = operand.x * c.x + operand.y * c.y + operand.z * c.z + operand.w * c.w;
        vec.w = operand.x * d.x + operand.y * d.y + operand.z * d.z + operand.w * d.w;

        return vec;
    }

    Mat4 Mat4::operator*(const Mat4& operand) {
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

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

    Mat4 Mat4::perspectiveProjection(float fieldOfView, RectObj extent, float far, float near) {
        SPLITGUI_PROFILE;

        Mat4 projection;

        float fov = std::tan(fieldOfView / 2);

        projection.a.x = 1.0f / fov * ((float)extent.height / (float)extent.width);
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

    void Mat4::updatePerspective(float fieldOfView, RectObj extent) {
        SPLITGUI_PROFILE;


        float fov = std::tan(fieldOfView / 2);

        a.x = (1.0f / fov) * ((float)extent.height / (float)extent.width);
    }

    Mat4 Mat4::quaternionMatrix(Vec4 quaternion) {
        SPLITGUI_PROFILE;

        Mat4 matrix;

        matrix.a.x = 1 - 2 * quaternion.y * quaternion.y - 2 * quaternion.z * quaternion.z;
        matrix.a.y = 2 * quaternion.x * quaternion.y - 2 * quaternion.z * quaternion.w;
        matrix.a.z = 2 * quaternion.x * quaternion.z + 2 * quaternion.y * quaternion.w;
        matrix.a.w = 0;

        matrix.b.x = 2 * quaternion.x * quaternion.y + 2 * quaternion.z * quaternion.w;
        matrix.b.y = 1 - 2 * quaternion.x * quaternion.x - 2 * quaternion.z * quaternion.z;
        matrix.b.z = 2 * quaternion.y * quaternion.z - 2 * quaternion.x * quaternion.w;
        matrix.b.w = 0;

        matrix.c.x = 2 * quaternion.x * quaternion.z - 2 * quaternion.y * quaternion.w;
        matrix.c.y = 2 * quaternion.y * quaternion.z + 2 * quaternion.x * quaternion.w;
        matrix.c.z = 1 - 2 * quaternion.x * quaternion.x - 2 * quaternion.y * quaternion.y;
        matrix.c.w = 0;

        matrix.d.x = 0;
        matrix.d.y = 0;
        matrix.d.z = 0;
        matrix.d.w = 1;

        return matrix;
    }

    Mat4 Mat4::ident() {
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

        return x <= point.x && y <= point.y && x + width >= point.x && y + height >= point.y;
    }

    bool RectObj::atLeftEdge(IVec2 point, int edgeWidth) {
        SPLITGUI_PROFILE;

        return x <= point.x && x + edgeWidth >= point.x;
    }

    bool RectObj::atRightEdge(IVec2 point, int edgeWidth) {
        SPLITGUI_PROFILE;

        return x + width - edgeWidth <= point.x && x + width >= point.x;
    }

    bool RectObj::atTopEdge(IVec2 point, int edgeWidth) {
        SPLITGUI_PROFILE;

        return y <= point.y && y + edgeWidth >= point.y;
    }

    bool RectObj::atBottomEdge(IVec2 point, int edgeWidth) {
        SPLITGUI_PROFILE;

        return y + height - edgeWidth <= point.y && y + height >= point.y;
    }

    bool RectObj::atEdge(IVec2 point, int edgeWidth) {
        SPLITGUI_PROFILE;

        return atLeftEdge(point, edgeWidth) || atRightEdge(point, edgeWidth) || atTopEdge(point, edgeWidth) || atBottomEdge(point, edgeWidth);
    }

    template <typename T>
    LinkList<T>::~LinkList() {
        SPLITGUI_PROFILE;

        clear();

        if (arrayPtr) {
            delete[] arrayPtr;
        }
    }

    template <typename T>
    void LinkList<T>::erase(LinkElement<T>* start, LinkElement<T>* end) {
        SPLITGUI_PROFILE;

        if (start->previous) {
            start->previous->next = end->next;
        } else {
            first = end->next;
        }

        if (end->next) {
            end->next->previous = start->previous;
        } else {
            last = start->previous;
        }

        LinkElement<T>* current = start;
        LinkElement<T>* stop = end->next;
        while (current != stop) {
            LinkElement<T>* next = current->next;
            delete current;
            current = next;
            listSize--;
        }
    }

    template <typename T>
    LinkElement<T>* LinkList<T>::push(T data) {
        SPLITGUI_PROFILE;

        LinkElement<T>* element = new LinkElement<T>;
        element->data = data;
        element->next = nullptr;
        element->previous = last;

        if (last) {
            last->next = element;
        } else {
            first = element;
        }

        last = element;
        listSize++;

        return element;
    }

    template <typename T>
    unsigned int LinkList<T>::size() {
        SPLITGUI_PROFILE;

        return listSize;
    }

    template <typename T>
    T* LinkList<T>::array() {
        SPLITGUI_PROFILE;

        if (arrayPtr) {
            delete[] arrayPtr;
            arrayPtr = nullptr;
        }

        arrayPtr = new T[listSize];

        LinkElement<T>* element = first;

        for (unsigned int i = 0; i < listSize && element; i++, element = element->next) {
            
            arrayPtr[i] = element->data;
        }

        return arrayPtr;
    }

    template <typename T>
    T* LinkList<T>::sliceArray(LinkElement<T>* start, LinkElement<T>* end) {
        SPLITGUI_PROFILE;

        if (arrayPtr) {
            delete[] arrayPtr;
            arrayPtr = nullptr;
        }

        unsigned int sliceSize = 0;

        LinkElement<T>* element = start;

        while (element != end->next && element) {

            sliceSize++;
            element = element->next;
        }

        arrayPtr = new T[sliceSize];

        element = start;

        for (unsigned int i = 0; i < sliceSize && element; i++, element = element->next) {
            
            arrayPtr[i] = element->data;
        }

        return arrayPtr;
    }

    template <typename T>
    void LinkList<T>::clear() {
        LinkElement<T>* element = first;

        while (element) {
            LinkElement<T>* next = element->next;
            delete element;
            element = next;
        }

        first = nullptr;
        last = nullptr;
    }

    template <typename T>
    std::optional<unsigned int> LinkList<T>::offset(LinkElement<T>* elementTest) {

        LinkElement<T>* element = first;

        for (unsigned int i = 0; i < listSize && element; i++, element = element->next) {
            if (element == elementTest) {
                return i;
            }
        }

        return std::nullopt;
    }
}