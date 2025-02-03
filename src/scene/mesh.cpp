#include <splitgui/scene.hpp>

namespace SplitGui {
    void Mesh::submit(int sceneNumber) {
        pGraphics->submitTriangleData(sceneNumber, vertices, indices);
    }
}