#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    Result Default::Meta::update() {
        return Result::eSuccess;
    }

    Result Default::Meta::instance() {

        SPLITGUI_LOG("Instanced Meta");

        return Result::eSuccess;
    }
}