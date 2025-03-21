#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    void Default::Text::setText(std::string valueIn) {
        value = valueIn;
    }

    void Default::Text::setColor(HexColor colorIn) {
        color = colorIn;
    }

    Result Default::Text::setColor(std::string colorIn) {

        colorStatement.checkCleanup();

        SplitGui::ResultValue<SplitGui::UnitExpression*> parseRes = colorStatement.parse(colorIn);
        TRYD(parseRes);

        if (parseRes.value->type != SplitGui::UnitExpression::Type::eVector || 
            parseRes.value->vector.isIVec != true                           || 
            parseRes.value->vector.size != 3) {

            return Result::eInvalidExpression;
        }

        UnitExpressionValue colorEval = colorStatement.evaluate(extent.height);
        color = colorEval.vector.ivec3;

        Logger::info("Set Text Color");

        return Result::eSuccess;
    }

    Result Default::Text::update() {
        return pGraphics->updateText(textRef, {extent.x, extent.y}, color, fontSize, depth);
    }

    Result Default::Text::instance() {

        SplitGui::ResultValue<TextRef> textRes = pGraphics->drawText({extent.x, extent.y}, value, color, fontSize, depth);

        TRYD(textRes);

        textRef = textRes.value;

        Logger::info("Instanced Text");

        return Result::eSuccess;
    }

    void Default::Text::setSize(unsigned int size) {
        fontSize = size;
    }
}