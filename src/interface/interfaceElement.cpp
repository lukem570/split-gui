#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result InterfaceElement::instance() {
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
        for (unsigned int i = 0; i < children.size(); i++) {
            TRYR(childrenRes, children[i]->submit());
        }

        return Result::eSuccess;
    }

    void InterfaceElement::addChild(InterfaceElement* child) {
        children.push_back(child);
    }

    void InterfaceElement::setSize(IVec2 size) {
        extent.width  = size.x;
        extent.height = size.y;
    }

    void InterfaceElement::setPosition(IVec2 position) {
        extent.x = position.x;
        extent.y = position.y;
    }

    void InterfaceElement::setExtent(RectObj extentIn) {
        extent = extentIn;
    }

    void InterfaceElement::setGraphics(Graphics* pGraphicsIn) {
        pGraphics = pGraphicsIn;
    }

    void InterfaceElement::setRef(std::string string) {
        ref = string;
    }

    void InterfaceElement::setDepth(int depthIn) {
        Logger::info("Depth: " + std::to_string(depthIn));
        depth = depthIn;
    }

    void InterfaceElement::cleanup() {
        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->cleanup();
            delete children[i];
        }
        children.clear();
    }

    std::vector<InterfaceElement*> InterfaceElement::searchByReference(std::string reference) {
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
        return extent;
    }
};