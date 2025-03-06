#include <splitgui/scene.hpp>

namespace SplitGui {
    void Mesh::submit(SceneRef& ref, int flags) {
        pGraphics->submitTriangleData(ref, vertices, indices, flags);
    }
}