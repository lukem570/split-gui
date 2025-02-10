#include <splitgui/structs.hpp>
#include <cmath>

namespace SplitGui {
    Mat3 Quaternion::convertToMatrix() {
        Mat3 matrix;

        matrix.a.x = 1 - 2 * (y * y + z * z);
        matrix.b.y = 2 * (x * y - z * w);
        matrix.c.z = 2 * (x * z + y * w);

        matrix.a.x = 2 * (x * y + z * w);
        matrix.b.y = 1 - 2 * (y * y + z * z);
        matrix.c.z = 2 * (x * z - y * w);

        matrix.a.x = 2 * (x * z - y * w);
        matrix.b.y = 2 * (y * z + x * w);
        matrix.c.z = 1 - 2 * (y * y + z * z);

        return matrix;
    }

    void Quaternion::rotate(Vec3 axis, float angle) {

    }
}