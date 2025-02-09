#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include "../unitExpression.cpp"

namespace SplitGui {
    void Default::Rect::update() {
        IVec2 x1;
        x1.x = extent.x;
        x1.y = extent.y;

        IVec2 x2;
        x2.x = extent.x + extent.width;
        x2.y = extent.y + extent.height;

        SPLITGUI_LOG("Updated Rect: (%d, %d), (%d, %d), p:%u", x1.x, x1.y, x2.x, x2.y, graphicsRectRef.bottomLeft);

        pGraphics->updateRect(graphicsRectRef, x1, x2);
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

        SPLITGUI_LOG("Created Rect: (%d, %d), (%d, %d), color: (%d, %d, %d)", x1.x, x1.y, x2.x, x2.y, color.r, color.g, color.b);

        graphicsRectRef = pGraphics->drawRect(x1, x2, color);

        return Result::eSuccess;
    }

    void Default::Rect::setColor(HexColor colorIn) {
        color = colorIn;
    }

    Result Default::Rect::setColor(std::string colorIn) {
        TRY(SplitGui::UnitExpression*, parseRes, colorStatement.parse(colorIn));
        // todo check size


        UnitExpressionValue colorEval = colorStatement.evaluate(extent.height);
        color = colorEval.vector.ivec3;

        SPLITGUI_LOG("Updated Rect Color: (%d, %d, %d)", color.r, color.g, color.b);

        return Result::eSuccess;
    }
}