#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result InterfaceElement::instance() {
        SPLITGUI_PROFILE;

        if (maxChildren < children.size()) {
            return Result::eInvalidNumberOfChildren;
        }

        for (unsigned int i = 0; i < children.size(); i++) {
            if (children[i]->type == InterfaceElementType::eMeta    ||
                children[i]->type == InterfaceElementType::eBinding) {

                continue;
            }

            children[i]->setGraphics(pGraphics);
            children[i]->setExtent(extent);
            children[i]->instance();
            break;
        }

        return Result::eSuccess;
    }

    Result InterfaceElement::update() {
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < children.size(); i++) {
            if (children[i]->type == InterfaceElementType::eMeta    ||
                children[i]->type == InterfaceElementType::eBinding) {

                continue;
            }

            children[i]->setExtent(extent);
            TRYR(updateRes, children[i]->update());
            break;
        }

        return Result::eSuccess;
    }

    Result InterfaceElement::submit() {
        SPLITGUI_PROFILE;
        
        for (unsigned int i = 0; i < children.size(); i++) {
            TRYR(childrenRes, children[i]->submit());
        }

        return Result::eSuccess;
    }

    void InterfaceElement::addChild(InterfaceElement* child) {
        SPLITGUI_PROFILE;

        children.push_back(child);
    }

    void InterfaceElement::clearChildren() {
        SPLITGUI_PROFILE;

        children.clear();
    }

    void InterfaceElement::setSize(IVec2 size) {
        SPLITGUI_PROFILE;

        extent.width  = size.x;
        extent.height = size.y;
    }

    void InterfaceElement::setPosition(IVec2 position) {
        SPLITGUI_PROFILE;

        extent.x = position.x;
        extent.y = position.y;
    }

    void InterfaceElement::setExtent(RectObj extentIn) {
        SPLITGUI_PROFILE;

        extent = extentIn;
    }

    void InterfaceElement::setGraphics(Graphics* pGraphicsIn) {
        SPLITGUI_PROFILE;

        pGraphics = pGraphicsIn;
    }

    void InterfaceElement::setRef(std::string string) {
        SPLITGUI_PROFILE;

        ref = string;
    }

    void InterfaceElement::setDepth(int depthIn) {
        SPLITGUI_PROFILE;

        Logger::info("Depth: " + std::to_string(depthIn));
        depth = depthIn;
    }

    void InterfaceElement::cleanup() {
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->cleanup();
            delete children[i];
        }
        children.clear();
    }

    std::vector<InterfaceElement*> InterfaceElement::searchByReference(std::string reference) {
        SPLITGUI_PROFILE;

        std::vector<InterfaceElement*> results;
        for (unsigned int i = 0; i < children.size(); i++) {
            std::vector<InterfaceElement*> childResults = children[i]->searchByReference(reference);
            results.insert(results.end(), childResults.begin(), childResults.end());
        }

        if (reference == ref) {
            results.push_back(this);
        }

        return results;
    }

    RectObj InterfaceElement::getExtent() {
        SPLITGUI_PROFILE;

        return extent;
    }
};