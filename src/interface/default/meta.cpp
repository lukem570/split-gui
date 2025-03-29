#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::Meta::update() {
        SPLITGUI_PROFILE;

        return Result::eSuccess;
    }

    Result Default::Meta::instance() {
        SPLITGUI_PROFILE;

        Logger::info("Instanced Meta");

        return Result::eSuccess;
    }
}