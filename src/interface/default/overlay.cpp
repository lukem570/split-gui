#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    Result Default::Overlay::update() {

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setExtent(extent);
            TRYR(updateRes, children[i]->update());
        }

        return Result::eSuccess;
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