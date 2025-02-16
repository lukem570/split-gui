#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Box::update() {
        children[0]->setExtent(Default::Box::extent);
        children[0]->update();
    }

    Result Default::Box::instance() {
        if (Default::Box::maxChildren < Default::Box::children.size()) {
            return Result::eInvalidNumberOfChildren;
        }

        children[0]->setGraphics(pGraphics);
        children[0]->setExtent(Default::Box::extent);
        children[0]->instance();

        SPLITGUI_LOG("Instanced Box");

        return Result::eSuccess;
    }
}