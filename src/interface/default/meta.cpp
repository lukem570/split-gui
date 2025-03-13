#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::Meta::update() {
        return Result::eSuccess;
    }

    Result Default::Meta::instance() {

        Logger::info("Instanced Meta");

        return Result::eSuccess;
    }
}