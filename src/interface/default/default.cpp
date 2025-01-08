#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>

namespace SplitGui {
    
    void Default::Split::instance() {
        if (Default::Split::maxChildren != Default::Split::children.size()) {
            printf("ERROR: '%s' element doesn't have 2 children", Default::Split::name.c_str());
            throw;
        }

        if (Default::Split::position > 0 && 1 > Default::Split::position) {
            printf("ERROR: position must be between 0 and 1");
            throw;
        }

        RectObj childExtentOne;
        RectObj childExtentTwo;

        int divide;

        if (Default::Split::isVertical) {
            divide = Default::Split::extent.height * Default::Split::position;

            childExtentOne.x      = Default::Split::extent.x;
            childExtentOne.y      = Default::Split::extent.y;
            childExtentOne.width  = Default::Split::extent.width;
            childExtentOne.height = divide;

            childExtentTwo.x      = Default::Split::extent.x;
            childExtentTwo.y      = Default::Split::extent.y + divide;
            childExtentTwo.width  = Default::Split::extent.width;
            childExtentTwo.height = Default::Split::extent.height - divide;

        } else {
            divide = Default::Split::extent.width * Default::Split::position;
            
            childExtentOne.x      = Default::Split::extent.x;
            childExtentOne.y      = Default::Split::extent.y;
            childExtentOne.width  = divide;
            childExtentOne.height = Default::Split::extent.height;

            childExtentTwo.x      = Default::Split::extent.x + divide;
            childExtentTwo.y      = Default::Split::extent.y;
            childExtentTwo.width  = Default::Split::extent.width - divide;
            childExtentTwo.height = Default::Split::extent.height;
        }
        
        Default::Split::children[0]->setGraphics(Default::Split::pGraphics);
        Default::Split::children[0]->setExtent(childExtentOne);
        Default::Split::children[0]->instance();

        Default::Split::children[1]->setGraphics(Default::Split::pGraphics);
        Default::Split::children[1]->setExtent(childExtentTwo);
        Default::Split::children[1]->instance();
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
        if (Default::Rect::maxChildren != Default::Rect::children.size()) {
            printf("ERROR: '%s' element doesn't have 2 children", Default::Rect::name.c_str());
            throw;
        }

        IVec2 x1;
        x1.x = Default::Rect::extent.x;
        x1.y = Default::Rect::extent.y;

        IVec2 x2;
        x2.x = Default::Rect::extent.x + Default::Rect::extent.width;
        x2.y = Default::Rect::extent.y + Default::Rect::extent.height;

        Default::Rect::pGraphics->drawRect(x1, x2, Default::Rect::color);
    }

    void Default::Rect::setColor(HexColor color) {
        Default::Rect::color = color;
    }
    
    void Default::Scene::instance() {
        
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