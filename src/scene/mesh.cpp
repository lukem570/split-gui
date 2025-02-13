#include <splitgui/scene.hpp>

namespace SplitGui {
    void Mesh::submit(int sceneNumber, int flags) {
        pGraphics->submitTriangleData(sceneNumber, vertices, indices, flags);
    }
}