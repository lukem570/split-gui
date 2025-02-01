#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Box::update() {
        
    }

    Result Default::Box::instance() {
        if (Default::Box::maxChildren < Default::Box::children.size()) {
            return Result::eInvalidNumberOfChildren;
        }

        
        Default::Box::children[0]->setExtent(Default::Box::extent);
        Default::Box::children[0]->instance();

        return Result::eSuccess;
    }
}