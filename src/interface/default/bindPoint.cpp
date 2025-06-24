#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Default::BindPoint::~BindPoint() {
        SPLITGUI_PROFILE;

        if (owner && root) {
            root->cleanup();
            ma::untrack(root);
            delete root;
        }
    }

    void Default::BindPoint::setBindPoint(InterfaceElement* rootIn) {
        SPLITGUI_PROFILE;

        root = rootIn;
    }

    void Default::BindPoint::setOwnership(bool ownerIn) {
        SPLITGUI_PROFILE;

        owner = ownerIn;
    }

    Result Default::BindPoint::instance() {
        SPLITGUI_PROFILE;

        root->setGraphics(pGraphics);
        root->setExtent(extent);
        root->instance();
        
        Logger::info("Instanced Bind Point");

        return Result::eSuccess;
    }

    Result Default::BindPoint::update() {
        SPLITGUI_PROFILE;

        root->setExtent(extent);
        return root->update();
    }

    std::vector<InterfaceElement*> Default::BindPoint::searchByReference(std::string reference) {
        SPLITGUI_PROFILE;

        std::vector<InterfaceElement*> results;
        if (root) {
            std::vector<InterfaceElement*> childResults = root->searchByReference(reference);
            results.insert(results.end(), childResults.begin(), childResults.end());
        }

        if (reference == ref) {
            results.push_back(this);
        }

        return results;
    }
}