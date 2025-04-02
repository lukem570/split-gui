#include <splitgui/scene.hpp>

#include <cmath>

namespace SplitGui {
    Result Camera::update(SceneRef& ref) {
        SPLITGUI_PROFILE;

        Mat4 rotationX = Mat4::xRotationMatrix(transform.rotation.x);
        Mat4 rotationY = Mat4::yRotationMatrix(transform.rotation.y);

        Mat4 rotation = rotationX * rotationY;

        view = rotation;

        pGraphics->updateSceneCameraView(ref, view);
        pGraphics->updateSceneCameraPosition(ref, transform.position);

        TRYR(updateRes, pGraphics->submitSceneData(ref));

        return Result::eSuccess;
    }

    Mat4& Camera::getView() {
        SPLITGUI_PROFILE;

        return view;
    }
}