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

    void Default::BindPoint::update() {
        root->setExtent(extent);
        root->update();
    }
}