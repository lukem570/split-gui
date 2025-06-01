#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::Switch::update() {
        SPLITGUI_PROFILE;

        if (hidden) {
            return Result::eSuccess;
        }

        if (children.empty()) {
            return Result::eSuccess;
        }

        children[active]->setExtent(extent);
        children[active]->setHidden(false);
        TRYR(updateRes, children[active]->update());

        for (unsigned int i = 0; i < children.size(); i++) {
            if (i != active) {
                TRYR(hiddenRes, children[i]->setHidden(true));
            }
        }

        return Result::eSuccess;
    }

    Result Default::Switch::instance() {
        SPLITGUI_PROFILE;

        if (hidden) {
            return Result::eSuccess;
        }

        if (children.empty()) {
            return Result::eSuccess;
        }

        children[active]->setGraphics(pGraphics);
        children[active]->setExtent(extent);
        children[active]->setHidden(false);
        TRYR(instRes, children[active]->instance());

        for (unsigned int i = 0; i < children.size(); i++) {
            if (i != active) {
                TRYR(hiddenRes, children[i]->setHidden(true));
            }
        }

        Logger::info("Instanced Switch");

        return Result::eSuccess;
    }

    void Default::Switch::setActive(unsigned int idx) {
        active = idx;
    }

    void Default::Switch::popBack() {
        children.back()->cleanup();
        delete children.back();

        children.pop_back();
    }

}