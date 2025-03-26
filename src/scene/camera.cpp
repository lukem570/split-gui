#include <splitgui/scene.hpp>

#include <cmath>

namespace SplitGui {
    Result Camera::update(SceneRef& ref) {

        Mat4 rotationX = Mat4::xRotationMatrix(transform.rotation.x);
        Mat4 rotationY = Mat4::yRotationMatrix(transform.rotation.y);
        Mat4 rotationZ = Mat4::zRotationMatrix(transform.rotation.z);

        Mat4 rotation = rotationX * (rotationY * rotationZ);

        view = rotation;

        TRYR(viewRes, pGraphics->updateSceneCameraView(ref, view));
        TRYR(posRes, pGraphics->updateSceneCameraPosition(ref, transform.position));

        return Result::eSuccess;
    }

    Mat4 Camera::getView() {
        return view;
    }
}