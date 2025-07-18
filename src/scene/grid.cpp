#include <splitgui/scene.hpp>

namespace SplitGui {

    Grid::Grid() {
        SPLITGUI_PROFILE;

        vertices.resize(4);
        indices.resize(12); // 2 for both sides
    }

    void Grid::setColor(HexColor colorIn) {
        SPLITGUI_PROFILE;

        color = colorIn;
    }

    void Grid::generate() {
        SPLITGUI_PROFILE;

        const float extent = 100.0f;

        vertices[0].pos.x = -extent;
        vertices[0].pos.z = -extent;
        vertices[0].pos.y = 0;
        vertices[0].color = color.normalize();
        vertices[0].textureCord = {0, 0};

        vertices[1].pos.x =  extent;
        vertices[1].pos.z = -extent;
        vertices[1].pos.y = 0;
        vertices[1].color = color.normalize();
        vertices[1].textureCord = {1, 0};

        vertices[2].pos.x = -extent;
        vertices[2].pos.z =  extent;
        vertices[2].pos.y = 0;
        vertices[2].color = color.normalize();
        vertices[2].textureCord = {0, 1};

        vertices[3].pos.x =  extent;
        vertices[3].pos.z =  extent;
        vertices[3].pos.y = 0;
        vertices[3].color = color.normalize();
        vertices[3].textureCord = {1, 1};

        int idx = 0;

        indices[idx++] = 0, indices[idx++] = 1, indices[idx++] = 3;
        indices[idx++] = 0, indices[idx++] = 3, indices[idx++] = 1;
        indices[idx++] = 0, indices[idx++] = 2, indices[idx++] = 3;
        indices[idx++] = 0, indices[idx++] = 3, indices[idx++] = 2;
    }

    Result Grid::submit(SceneRef& ref, int flags) {
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