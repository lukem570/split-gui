#include <splitgui/scene.hpp>

namespace SplitGui {
    Result Mesh::submit(SceneRef& ref, int flags) {
        SPLITGUI_PROFILE;

        if (!model.has_value()) {
            ResultValue<ModelRef> modelRef = pGraphics->createModel(ref, transform.getModel());
            TRYD(modelRef);

            model = modelRef.value;
        }

        ResultValue<TriangleRef> triangleRes = pGraphics->submitTriangleData(ref, vertices, indices, model.value(), flags);

        TRYD(triangleRes);

        triangleRef = triangleRes.value;

        return Result::eSuccess;
    }
}