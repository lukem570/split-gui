#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    void Default::Text::setText(std::string valueIn) {
        value = valueIn;
    }

    void Default::Text::setColor(HexColor colorIn) {
        color = colorIn;
    }

    Result Default::Text::setColor(std::string colorIn) {
        SplitGui::ResultValue<SplitGui::UnitExpression*> parseRes = colorStatement.parse(colorIn);
        TRYD(parseRes);

        if (parseRes.value->type != SplitGui::UnitExpression::Type::eVector || 
            parseRes.value->vector.isIVec != true                           || 
            parseRes.value->vector.size != 3) {

            return Result::eInvalidExpression;
        }


        UnitExpressionValue colorEval = colorStatement.evaluate(extent.height);
        color = colorEval.vector.ivec3;

        SPLITGUI_LOG("Set Text Color: (%d, %d, %d)", color.r, color.g, color.b);

        return Result::eSuccess;
    }

    void Default::Text::update() {
        // TODO:
    }

    Result Default::Text::instance() {

        SplitGui::ResultValue<int> textRes = pGraphics->drawText({extent.x, extent.y}, value, color, depth);
        TRYD(textRes);

        SPLITGUI_LOG("Instanced Text");

        return Result::eSuccess;
    }
}