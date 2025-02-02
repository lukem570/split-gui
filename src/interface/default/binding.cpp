#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Binding::update() {
        
    }

    Result Default::Binding::instance() {
        return Result::eSuccess;
    }

    void Default::Binding::setAlias(std::string& inAlias) {
        alias = inAlias;
    }
}