#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Meta::update() {
        
    }

    Result Default::Meta::instance() {
        return Result::eSuccess;
    }
}