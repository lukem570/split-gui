#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::SceneElement::update() {
        pGraphics->updateScene(graphicsSceneRef, extent.pos, extent.pos + extent.size);
    }
    
    Result Default::SceneElement::instance() {
        graphicsSceneRef = pGraphics->instanceScene(extent.pos, extent.pos + extent.size);

        SPLITGUI_LOG("Instanced Scene");

        return Result::eSuccess;
    }

    void Default::SceneElement::setSceneNumber(unsigned int sceneNumber) {
        number = sceneNumber;
    }
}