#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Binding::update() {
        
    }

    Result Default::Binding::instance() {

        SPLITGUI_LOG("Instanced Binding");

        return Result::eSuccess;
    }

    void Default::Binding::setAlias(std::string& inAlias) {
        alias = inAlias;
    }

    void Default::Binding::setSource(std::string& inSource) {
        source = inSource;
    }

    std::string Default::Binding::getAlias() {
        return alias;
    }

    std::string Default::Binding::getSource() {
        return source;
    }

}