#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    
    Result Default::Split::update() {
        SPLITGUI_PROFILE;
        
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
        
        children[0]->setExtent(childExtentOne);
        TRYR(updateRes1, children[0]->update());

        children[1]->setExtent(childExtentTwo);
        TRYR(updateRes2, children[1]->update());

        return Result::eSuccess;
    }

    Result Default::Split::instance() {
        SPLITGUI_PROFILE;

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
        
        Logger::info("Instanced Split");

        children[0]->setGraphics(pGraphics);
        children[0]->setExtent(childExtentOne);
        TRYR(instanceRes1, children[0]->instance());

        children[1]->setGraphics(pGraphics);
        children[1]->setExtent(childExtentTwo);
        TRYR(instanceRes2, children[1]->instance());

        return Result::eSuccess;
    }

    void Default::Split::setVertical(bool state) {
        SPLITGUI_PROFILE;

        isVertical = state;
    }

    Result Default::Split::setPosition(std::string pos) {
        SPLITGUI_PROFILE;

        position.checkCleanup();

        TRY(SplitGui::UnitExpression*, parseRes, position.parse(pos));

        return Result::eSuccess;
    }
}