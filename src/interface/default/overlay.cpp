#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::Overlay::update() {
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setExtent(extent);
            TRYR(updateRes, children[i]->update());
        }

        return Result::eSuccess;
    }

    Result Default::Overlay::instance() {
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setGraphics(pGraphics);
            children[i]->setExtent(extent);
            children[i]->instance();
        }

        Logger::info("Instanced Overlay");

        return Result::eSuccess;
    }

}