#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <logutil/logutil.hpp>

namespace SplitGui {
    Result Default::Meta::update() {
        SPLITGUI_PROFILE;

        return Result::eSuccess;
    }

    Result Default::Meta::instance() {
        SPLITGUI_PROFILE;

        Logutil::info("Instanced Meta");

        return Result::eSuccess;
    }
}