#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    Result Default::Box::update() {
        children[0]->setExtent(extent);
        return children[0]->update();
    }

    Result Default::Box::instance() {
        if (maxChildren < children.size()) {
            return Result::eInvalidNumberOfChildren;
        } else if (children.size() == 0) {
            return Result::eSuccess;
        }

        children[0]->setGraphics(pGraphics);
        children[0]->setExtent(Default::Box::extent);
        children[0]->instance();

        SPLITGUI_LOG("Instanced Box");

        return Result::eSuccess;
    }
}