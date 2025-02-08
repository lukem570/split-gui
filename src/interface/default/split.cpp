#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {
    
    void Default::Split::update() {
        RectObj childExtentOne;
        RectObj childExtentTwo;

        UnitExpressionValue divide;

        if (isVertical) {
            divide = position.evaluate(extent.height);

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide.number;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide.number;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide.number;

        } else {
            divide = position.evaluate(extent.width);
            
            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = divide.number;
            childExtentOne.height = extent.height;

            childExtentTwo.x      = extent.x + divide.number;
            childExtentTwo.y      = extent.y;
            childExtentTwo.width  = extent.width - divide.number;
            childExtentTwo.height = extent.height;
        }
        
        printf("split update: e.w:%d e.h:%d v?:%d, d:%f\n", extent.width, extent.height, isVertical, divide.number);

        children[0]->setExtent(childExtentOne);
        children[0]->update();

        children[1]->setExtent(childExtentTwo);
        children[1]->update();
    }

    Result Default::Split::instance() {
        if (maxChildren != children.size()) {
            return Result::eInvalidNumberOfChildren;
        }


        RectObj childExtentOne;
        RectObj childExtentTwo;

        UnitExpressionValue divide;

        if (isVertical) {
            divide = position.evaluate(extent.height);

            if (divide.type != UnitExpressionValue::Type::eNumber) {
                return Result::eInvalidType;
            }

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide.number;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide.number;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide.number;

        } else {
            divide = position.evaluate(extent.width);

            if (divide.type != UnitExpressionValue::Type::eNumber) {
                return Result::eInvalidType;
            }
            
            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = divide.number;
            childExtentOne.height = extent.height;

            childExtentTwo.x      = extent.x + divide.number;
            childExtentTwo.y      = extent.y;
            childExtentTwo.width  = extent.width - divide.number;
            childExtentTwo.height = extent.height;
        }
        
        printf("split\n");

        children[0]->setGraphics(pGraphics);
        children[0]->setExtent(childExtentOne);
        children[0]->instance();

        children[1]->setGraphics(pGraphics);
        children[1]->setExtent(childExtentTwo);
        children[1]->instance();

        return Result::eSuccess;
    }

    void Default::Split::setVertical(bool state) {
        isVertical = state;
    }

    Result Default::Split::setPosition(std::string pos) {
        TRY(SplitGui::UnitExpression*, parseRes, position.parse(pos));

        return Result::eSuccess;
    }
}