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
        
        children[active]->setHidden(false);
        
        for (unsigned int i = 0; i < children.size(); i++) {
            if (i != active) {
                TRYR(hiddenRes, children[i]->setHidden(true));
            }

            children[i]->setExtent(extent);
            TRYR(updateRes, children[i]->update());
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
        
        children[active]->setHidden(false);
        
        for (unsigned int i = 0; i < children.size(); i++) {
            if (i != active) {
                TRYR(hiddenRes, children[i]->setHidden(true));
            }

            children[i]->setExtent(extent);
            children[i]->setGraphics(pGraphics);
            TRYR(instRes, children[i]->instance());
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