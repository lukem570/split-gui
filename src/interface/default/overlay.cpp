#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Overlay::update() {

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setExtent(extent);
            children[i]->update();
        }

    }

    Result Default::Overlay::instance() {

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setGraphics(pGraphics);
            children[i]->setExtent(extent);
            children[i]->instance();
        }

        SPLITGUI_LOG("Instanced Overlay");

        return Result::eSuccess;
    }

}