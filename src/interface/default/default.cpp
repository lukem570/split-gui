#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>

namespace SplitGui {
    
    void Default::Split::update() {
        RectObj childExtentOne;
        RectObj childExtentTwo;

        int divide;

        if (isVertical) {
            divide = evaluateUnitExpression(position, extent.height);

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide;

        } else {
            divide = evaluateUnitExpression(position, extent.width);
            
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

    void Default::Split::instance() {
        if (maxChildren != children.size()) {
            printf("ERROR: '%s' element doesn't have 2 children\n", name.c_str());
            fflush(stdout);
            throw;
        }


        RectObj childExtentOne;
        RectObj childExtentTwo;

        int divide;

        if (isVertical) {
            divide = evaluateUnitExpression(position, extent.height);

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide;

        } else {
            divide = evaluateUnitExpression(position, extent.width);
            
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
    }

    void Default::Split::setVertical(bool state) {
        isVertical = state;
    }

    void Default::Split::setPosition(std::string pos) {
        position = pos;
    }

    void Default::List::update() {
        
    }

    void Default::List::instance() {
        
    }

    void Default::Box::update() {
        
    }

    void Default::Box::instance() {
        if (Default::Box::maxChildren < Default::Box::children.size()) {
            printf("ERROR: '%s' element has too many children", Default::Box::name.c_str());
            throw;
        }

        
        Default::Box::children[0]->setExtent(Default::Box::extent);
        Default::Box::children[0]->instance();
    }

    void Default::Overlay::update() {
        
    }

    void Default::Overlay::instance() {
        
    }

    void Default::Mask::update() {
        
    }

    void Default::Mask::instance() {
        
    }

    void Default::Transform::update() {
        
    }

    void Default::Transform::instance() {
        
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

    void Default::Rect::instance() {
        if (maxChildren != children.size()) {
            printf("ERROR: '%s' element doesn't have 2 children", name.c_str());
            throw;
        }

        IVec2 x1;
        x1.x = extent.x;
        x1.y = extent.y;

        IVec2 x2;
        x2.x = extent.x + extent.width;
        x2.y = extent.y + extent.height;

        printf("rect: (%d, %d), (%d, %d), color: (%d, %d, %d)\n", x1.x, x1.y, x2.x, x2.y, color.r, color.g, color.b);

        graphicsRectRef = pGraphics->drawRect(x1, x2, color);
    }

    void Default::Rect::setColor(HexColor colorIn) {
        color = colorIn;
    }

    void Default::SceneElement::update() {
        pGraphics->updateScene(graphicsSceneRef, extent.pos, extent.pos + extent.size);
    }
    
    void Default::SceneElement::instance() {
        graphicsSceneRef = pGraphics->instanceScene(extent.pos, extent.pos + extent.size);
    }

    void Default::SceneElement::setSceneNumber(unsigned int sceneNumber) {
        number = sceneNumber;
    }

    void Default::Text::update() {
        
    }

    void Default::Text::instance() {
        
    }

    void Default::Media::update() {
        
    }

    void Default::Media::instance() {
        
    }

    void Default::Binding::update() {
        
    }

    void Default::Binding::instance() {
        
    }

    void Default::Meta::update() {
        
    }

    void Default::Meta::instance() {
        
    }
}