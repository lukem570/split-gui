#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    Result Default::SceneElement::update() {
        return pGraphics->updateScene(graphicsSceneRef, extent.pos, extent.pos + extent.size);
    }
    
    Result Default::SceneElement::instance() {
        ResultValue<SceneRef> result = pGraphics->instanceScene(extent.pos, extent.pos + extent.size);

        TRYD(result);

        graphicsSceneRef = result.value;

        SPLITGUI_LOG("Instanced Scene");

        return Result::eSuccess;
    }

    SceneRef Default::SceneElement::getSceneRef() {
        return graphicsSceneRef;
    }
}