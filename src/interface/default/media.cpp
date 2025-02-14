#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Media::update() {
        
    }

    Result Default::Media::instance() {

        SPLITGUI_LOG("Instanced Media");

        return Result::eSuccess;
    }
}