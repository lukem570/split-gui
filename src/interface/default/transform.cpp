#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Transform::update() {
        
    }

    Result Default::Transform::instance() {

        SPLITGUI_LOG("Instanced Transform");

        return Result::eSuccess;
    }
}