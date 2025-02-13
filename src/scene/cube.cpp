#include <splitgui/scene.hpp>

namespace SplitGui {
    Cube::Cube() {
        vertices.resize(8);
        indices.resize(6 * 2 * 3);
    }

    void Cube::setSize(float sizeIn) {
        size = sizeIn;
    }

    void Cube::setColor(HexColor colorIn) {
        color = colorIn;
    }

    void Cube::generate() {

        int idx = 0;

        for (int x = -1; x <= 1; x += 2) {
            for (int y = -1; y <= 1; y += 2) {
                for (int z = -1; z <= 1; z += 2) {
                    vertices[idx].pos     = Vec3{x * size, y * size, z * size} + transform.position;
                    vertices[idx++].color = color.normalize();
                }
            }
        }

        idx = 0;

        indices[idx++] = 0, indices[idx++] = 1, indices[idx++] = 2;
        indices[idx++] = 1, indices[idx++] = 3, indices[idx++] = 2;

        indices[idx++] = 4, indices[idx++] = 6, indices[idx++] = 5;
        indices[idx++] = 5, indices[idx++] = 6, indices[idx++] = 7;

        indices[idx++] = 2, indices[idx++] = 3, indices[idx++] = 6;
        indices[idx++] = 3, indices[idx++] = 7, indices[idx++] = 6;

        indices[idx++] = 5, indices[idx++] = 0, indices[idx++] = 4;
        indices[idx++] = 1, indices[idx++] = 0, indices[idx++] = 5;

        indices[idx++] = 1, indices[idx++] = 5, indices[idx++] = 3;
        indices[idx++] = 5, indices[idx++] = 7, indices[idx++] = 3;

        indices[idx++] = 0, indices[idx++] = 2, indices[idx++] = 4;
        indices[idx++] = 2, indices[idx++] = 6, indices[idx++] = 4;
    }
}