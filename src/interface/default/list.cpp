#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    Result Default::List::update() {
        SPLITGUI_PROFILE;

        int maxSize = isVertical ? extent.height : extent.width;
        int pos     = isVertical ? extent.y : extent.x;
        int size    = maxSize / children.size();

        std::vector<RectObj> extents;
        extents.resize(children.size());

        for (unsigned int i = 0; i < children.size(); i++) {
            
            if (isVertical) {

                extents[i].height = size;
                extents[i].width  = extent.width;
                extents[i].x      = extent.x;
                extents[i].y      = pos;
                
            } else {

                extents[i].height = extent.height;
                extents[i].width  = size;
                extents[i].y      = extent.y;
                extents[i].x      = pos;
                
            }

            pos += size;
        }
        
        if (isVertical) {
            extents.back().height += maxSize + extent.y - pos;
        } else {
            extents.back().width  += maxSize + extent.x - pos;
        }

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setExtent(extents[i]);
            TRYR(updateRes, children[i]->update());
        }

        return Result::eSuccess;
    }

    Result Default::List::instance() {
        SPLITGUI_PROFILE;

        int maxSize = isVertical ? extent.height : extent.width;
        int pos     = isVertical ? extent.y : extent.x;
        int size    = maxSize / children.size();

        std::vector<RectObj> extents;
        extents.resize(children.size());

        for (unsigned int i = 0; i < children.size(); i++) {
            
            if (isVertical) {

                extents[i].height = size;
                extents[i].width  = extent.width;
                extents[i].x      = extent.x;
                extents[i].y      = pos;
                
            } else {

                extents[i].height = extent.height;
                extents[i].width  = size;
                extents[i].y      = extent.y;
                extents[i].x      = pos;

            }

            pos += size;
        }
        
        if (isVertical) {
            extents.back().height += maxSize + extent.y - pos;
        } else {
            extents.back().width  += maxSize + extent.x - pos;
        }

        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setGraphics(pGraphics);
            children[i]->setExtent(extents[i]);
            TRYR(instanceRes, children[i]->instance());
        }
        
        Logger::info("Instanced List");

        return Result::eSuccess;
    }

    void Default::List::setVertical(bool verticalIn) {
        SPLITGUI_PROFILE;

        isVertical = verticalIn;
    }
}