#include <splitgui/scene.hpp>

namespace SplitGui {
    Result Mesh::submit(SceneRef& ref, int flags) {

        ResultValue<TriangleRef> triangleRes = pGraphics->submitTriangleData(ref, vertices, indices, flags);

        TRYD(triangleRes);

        triangleRef = triangleRes.value;

        return Result::eSuccess;
    }
}