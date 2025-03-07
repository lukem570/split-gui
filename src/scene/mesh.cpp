#include <splitgui/scene.hpp>

namespace SplitGui {
    Result Mesh::submit(SceneRef& ref, int flags) {
        return pGraphics->submitTriangleData(ref, vertices, indices, flags);
    }
}