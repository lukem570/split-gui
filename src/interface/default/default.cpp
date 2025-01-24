#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>

namespace SplitGui {
    
    void Default::Split::instance() {
        if (maxChildren != children.size()) {
            printf("ERROR: '%s' element doesn't have 2 children\n", name.c_str());
            fflush(stdout);
            throw;
        }

        if (position < 0.0f || position > 1.0f) {
            printf("ERROR: position must be between 0 and 1 it is: %.4f\n", position);
            fflush(stdout);
            throw;
        }

        RectObj childExtentOne;
        RectObj childExtentTwo;

        int divide;

        if (isVertical) {
            divide = extent.height * position;

            childExtentOne.x      = extent.x;
            childExtentOne.y      = extent.y;
            childExtentOne.width  = extent.width;
            childExtentOne.height = divide;

            childExtentTwo.x      = extent.x;
            childExtentTwo.y      = extent.y + divide;
            childExtentTwo.width  = extent.width;
            childExtentTwo.height = extent.height - divide;

        } else {
            divide = extent.width * position;
            
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

    void Default::List::instance() {
        
    }

    void Default::Box::instance() {
        if (Default::Box::maxChildren < Default::Box::children.size()) {
            printf("ERROR: '%s' element has too many children", Default::Box::name.c_str());
            throw;
        }

        
        Default::Box::children[0]->setExtent(Default::Box::extent);
        Default::Box::children[0]->instance();
    }

    void Default::Overlay::instance() {
        
    }

    void Default::Mask::instance() {
        
    }

    void Default::Transform::instance() {
        
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

        pGraphics->drawRect(x1, x2, color);
    }

    void Default::Rect::setColor(HexColor colorIn) {
        color = colorIn;
    }
    
    void Default::Scene::instance() {
        pGraphics->instanceScene(extent.pos, extent.pos + extent.size);
    }

    void Default::Scene::setSceneNumber(unsigned int sceneNumber) {
        number = sceneNumber;
    }

    void Default::Text::instance() {
        
    }

    void Default::Media::instance() {
        
    }

    void Default::Binding::instance() {
        
    }

    void Default::Meta::instance() {
        
    }
}