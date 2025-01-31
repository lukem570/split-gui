#include <splitgui/result.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>

namespace SplitGui {
    
    void Default::Split::update() {
        RectObj childExtentOne;
        RectObj childExtentTwo;

        int divide;

        if (isVertical) {
            divide = position.evaluate(extent.height);

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide;

        } else {
            divide = position.evaluate(extent.width);
            
            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = divide;
            childExtentOne.height = extent.height;

            childExtentTwo.x      = extent.x + divide;
            childExtentTwo.y      = extent.y;
            childExtentTwo.width  = extent.width - divide;
            childExtentTwo.height = extent.height;
        }
        
        printf("split update: e.w:%d e.h:%d v?:%d, d:%d\n", extent.width, extent.height, isVertical, divide);

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

        int divide;

        if (isVertical) {
            divide = position.evaluate(extent.height);

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide;

        } else {
            divide = position.evaluate(extent.width);
            
            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = divide;
            childExtentOne.height = extent.height;

            childExtentTwo.x      = extent.x + divide;
            childExtentTwo.y      = extent.y;
            childExtentTwo.width  = extent.width - divide;
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

    void Default::List::update() {
        
    }

    Result Default::List::instance() {
        return Result::eSuccess;
    }

    void Default::Box::update() {
        
    }

    Result Default::Box::instance() {
        if (Default::Box::maxChildren < Default::Box::children.size()) {
            return Result::eInvalidNumberOfChildren;
        }

        
        Default::Box::children[0]->setExtent(Default::Box::extent);
        Default::Box::children[0]->instance();

        return Result::eSuccess;
    }

    void Default::Overlay::update() {
        
    }

    Result Default::Overlay::instance() {
        return Result::eSuccess;
    }

    void Default::Mask::update() {
        
    }

    Result Default::Mask::instance() {
        return Result::eSuccess;
    }

    void Default::Transform::update() {
        
    }

    Result Default::Transform::instance() {
        return Result::eSuccess;
    }

    void Default::Rect::update() {
        IVec2 x1;
        x1.x = extent.x;
        x1.y = extent.y;

        IVec2 x2;
        x2.x = extent.x + extent.width;
        x2.y = extent.y + extent.height;

        printf("rect update: (%d, %d), (%d, %d), p:%u\n", x1.x, x1.y, x2.x, x2.y, graphicsRectRef.bottomLeft);

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

        printf("rect: (%d, %d), (%d, %d), color: (%d, %d, %d)\n", x1.x, x1.y, x2.x, x2.y, color.r, color.g, color.b);

        graphicsRectRef = pGraphics->drawRect(x1, x2, color);

        return Result::eSuccess;
    }

    void Default::Rect::setColor(HexColor colorIn) {
        color = colorIn;
    }

    void Default::SceneElement::update() {
        pGraphics->updateScene(graphicsSceneRef, extent.pos, extent.pos + extent.size);
    }
    
    Result Default::SceneElement::instance() {
        graphicsSceneRef = pGraphics->instanceScene(extent.pos, extent.pos + extent.size);

        return Result::eSuccess;
    }

    void Default::SceneElement::setSceneNumber(unsigned int sceneNumber) {
        number = sceneNumber;
    }

    void Default::Text::update() {
        
    }

    Result Default::Text::instance() {
        return Result::eSuccess;
    }

    void Default::Media::update() {
        
    }

    Result Default::Media::instance() {
        return Result::eSuccess;
    }

    void Default::Binding::update() {
        
    }

    Result Default::Binding::instance() {
        return Result::eSuccess;
    }

    void Default::Meta::update() {
        
    }

    Result Default::Meta::instance() {
        return Result::eSuccess;
    }
}