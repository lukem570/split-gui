#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

#include "../unitExpression.cpp"

namespace SplitGui {
    Result Default::Rect::update() {
        IVec2 x1;
        x1.x = extent.x;
        x1.y = extent.y;

        IVec2 x2;
        x2.x = extent.x + extent.width;
        x2.y = extent.y + extent.height;

        pGraphics->updateRect(graphicsRectRef, x1, x2, color, depth);

        return Result::eSuccess;
    }

    Result Default::Rect::instance() {
        if (maxChildren != children.size()) {
            return Result::eInvalidNumberOfChildren;
        }

        IVec2 x1;
        x1.x = extent.x;
        x1.y = extent.y;

        IVec2 x2;
        x2.x = extent.x + extent.width;
        x2.y = extent.y + extent.height;

        Logger::info("Instanced Rect");

        graphicsRectRef = pGraphics->drawRect(x1, x2, color, depth, flags, textureIndex);

        return Result::eSuccess;
    }

    Result Default::Rect::submit() {
        return pGraphics->submitRect(graphicsRectRef);
    }

    void Default::Rect::setColor(HexColor colorIn) {
        color = colorIn;
    }

    Result Default::Rect::setColor(std::string colorIn) {

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

        return Result::eSuccess;
    }

    void Default::Rect::setFlags(VertexFlags flagsIn) {
        flags = flagsIn;
    }

    void Default::Rect::setTextureIndex(int idxIn) {
        textureIndex = idxIn;
    }

}