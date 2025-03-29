#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::Box::update() {
        SPLITGUI_PROFILE;

        if (children.size() == 0) {
            return Result::eSuccess;
        }

        children[0]->setExtent(extent);
        return children[0]->update();
    }

    Result Default::Box::instance() {
        SPLITGUI_PROFILE;

        if (maxChildren < children.size()) {
            return Result::eInvalidNumberOfChildren;
        } else if (children.size() == 0) {
            return Result::eSuccess;
        }

        children[0]->setGraphics(pGraphics);
        children[0]->setExtent(Default::Box::extent);
        children[0]->instance();

        Logger::info("Instanced Box");

        return Result::eSuccess;
    }
}