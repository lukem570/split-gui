#include <splitgui/scene.hpp>

#include <cmath>

namespace SplitGui {
    void Camera::update() {

        float cosX = std::cos(transform.rotation.x);
        float sinX = std::sin(transform.rotation.x);

        float cosY = std::cos(transform.rotation.y);
        float sinY = std::sin(transform.rotation.y);

        forward = {
            cosX * sinY,
            sinX,
            cosX * cosY,
        };
        forward.normalize();

        right = Vec3{0.0f, 1.0f, 0.0f}.cross(forward);
        right.normalize();

        up = forward.cross(right);
        up.normalize();
    }

    Mat4 Camera::getView() {
        Mat4 view;

        float dotRight = right.dot(transform.position);
        float dotUp = up.dot(transform.position);
        float dotForward = forward.dot(transform.position);

        view.a.x = right.x, view.a.y = right.y, view.a.z = right.z, view.a.w = -dotRight;
        view.b.x = up.x, view.b.y = up.y, view.b.z = up.z, view.b.w = -dotUp;
        view.c.x = forward.x, view.c.y = forward.y, view.c.z = forward.z, view.c.w = dotForward;
        view.c.x = 0, view.c.y = 0, view.c.z = 0, view.c.w = 1;

        return view;
    }
}