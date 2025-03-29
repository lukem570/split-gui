#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::Binding::update() {
        SPLITGUI_PROFILE;

        return Result::eSuccess;
    }

    Result Default::Binding::instance() {
        SPLITGUI_PROFILE;


        Logger::info("Instanced Binding");

        return Result::eSuccess;
    }

    void Default::Binding::setAlias(std::string& inAlias) {
        SPLITGUI_PROFILE;

        alias = inAlias;
    }

    void Default::Binding::setSource(std::string& inSource) {
        SPLITGUI_PROFILE;

        source = inSource;
    }

    std::string Default::Binding::getAlias() {
        SPLITGUI_PROFILE;

        return alias;
    }

    std::string Default::Binding::getSource() {
        SPLITGUI_PROFILE;

        return source;
    }

}