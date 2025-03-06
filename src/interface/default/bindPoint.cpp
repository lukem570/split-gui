#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    Default::BindPoint::~BindPoint() {
        if (owner && root) {
            delete root;
        }
    }

    void Default::BindPoint::setBindPoint(InterfaceElement* rootIn) {
        root = rootIn;
    }

    void Default::BindPoint::setOwnership(bool ownerIn) {
        owner = ownerIn;
    }

    Result Default::BindPoint::instance() {
        root->setGraphics(pGraphics);
        root->setExtent(extent);
        root->instance();
        
        SPLITGUI_LOG("Instanced Bind Point");

        return Result::eSuccess;
    }

    Result Default::BindPoint::update() {
        root->setExtent(extent);
        return root->update();
    }

    std::vector<InterfaceElement*> Default::BindPoint::searchByReference(std::string reference) {
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