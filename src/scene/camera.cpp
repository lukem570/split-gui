#include <splitgui/scene.hpp>

#include <cmath>

namespace SplitGui {
    Result Camera::update(int sceneNumber) {

        Mat4 rotationX = Mat4::xRotationMatrix(transform.rotation.x);
        Mat4 rotationY = Mat4::yRotationMatrix(transform.rotation.y);

        Mat4 rotation = rotationX * rotationY;

        view = rotation;

        TRYR(viewRes, pGraphics->updateSceneCameraView(0, view));
        TRYR(posRes, pGraphics->updateSceneCameraPosition(0, transform.position));

        return Result::eSuccess;
    }

    Mat4 Camera::getView() {
        return view;
    }
}