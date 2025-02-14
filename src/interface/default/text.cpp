#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Text::setText(std::string valueIn) {
        value = valueIn;
    }

    void Default::Text::update() {
        // TODO:
    }

    Result Default::Text::instance() {

        SplitGui::ResultValue<int> textRes = pGraphics->drawText({extent.x, extent.y}, value);
        TRYD(textRes);

        SPLITGUI_LOG("Instanced Text");

        return Result::eSuccess;
    }
}