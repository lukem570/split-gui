#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::SceneElement::update() {
        return pGraphics->updateScene(graphicsSceneRef, extent.pos, extent.pos + extent.size, depth);
    }
    
    Result Default::SceneElement::instance() {
        ResultValue<SceneRef> result = pGraphics->instanceScene(extent.pos, extent.pos + extent.size, depth);

        TRYD(result);

        graphicsSceneRef = result.value;

        Logger::info("Instanced Scene");

        return Result::eSuccess;
    }

    SceneRef& Default::SceneElement::getSceneRef() {
        return graphicsSceneRef;
    }
}