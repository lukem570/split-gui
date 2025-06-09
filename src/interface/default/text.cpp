#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/logger.hpp>

namespace SplitGui {
    void Default::Text::setText(std::string valueIn) {
        SPLITGUI_PROFILE;
        
        value = valueIn;
    }

    void Default::Text::setColor(HexColor colorIn) {
        SPLITGUI_PROFILE;

        color = colorIn;
    }

    Result Default::Text::setColor(std::string colorIn) {
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

        if (hidden) {
            return Result::eSuccess;
        }

        ResultValue<IVec2> textSizeRes = pGraphics->getTextSize(value, fontSize);
        TRYD(textSizeRes);

        IVec2 origin;

        switch (horizontalAnchor) {
            case HorizontalAnchor::eLeft: {
                
                origin.x = extent.x;

                break;
            }
            case HorizontalAnchor::eCenter: {

                origin.x = extent.x + extent.width / 2;
                origin.x -= textSizeRes.value.x / 2;

                break;
            }
            case HorizontalAnchor::eRight: {

                origin.x = extent.x + extent.width;
                origin.x -= textSizeRes.value.x;

                break;
            }
        }

        switch (verticalAnchor) {
            case VerticalAnchor::eTop: {

                origin.y = extent.y;

                break;
            }
            case VerticalAnchor::eCenter: {

                origin.y = extent.y + extent.height / 2;
                origin.y -= textSizeRes.value.y / 2;

                break;
            }
            case VerticalAnchor::eBottom: {

                origin.y = extent.y + extent.height;
                origin.y -= textSizeRes.value.y;

                break;
            }
        }

        return pGraphics->updateText(textRef, origin, color, fontSize, depth);
    }

    Result Default::Text::instance() {
        SPLITGUI_PROFILE;

        if (hidden) {
            return Result::eSuccess;
        }

        ResultValue<IVec2> textSizeRes = pGraphics->getTextSize(value, fontSize);
        TRYD(textSizeRes);

        IVec2 origin;

        switch (horizontalAnchor) {
            case HorizontalAnchor::eLeft: {
                
                origin.x = extent.x;

                break;
            }
            case HorizontalAnchor::eCenter: {

                origin.x = extent.x + extent.width / 2;
                origin.x -= textSizeRes.value.x / 2;

                break;
            }
            case HorizontalAnchor::eRight: {

                origin.x = extent.x + extent.width;
                origin.x -= textSizeRes.value.x;

                break;
            }
        }

        switch (verticalAnchor) {
            case VerticalAnchor::eTop: {

                origin.y = extent.y;

                break;
            }
            case VerticalAnchor::eCenter: {

                origin.y = extent.y + extent.height / 2;
                origin.y -= textSizeRes.value.y / 2;

                break;
            }
            case VerticalAnchor::eBottom: {

                origin.y = extent.y + extent.height;
                origin.y -= textSizeRes.value.y;

                break;
            }
        }

        ResultValue<TextRef> textRes = pGraphics->drawText(origin, value, color, fontSize, depth);
        TRYD(textRes);

        textRef = textRes.value;

        textExists = true;

        Logger::info("Instanced Text");

        return Result::eSuccess;
    }

    Result Default::Text::setHidden(bool isHidden) {
        SPLITGUI_PROFILE;
        
        if (hidden != isHidden && isHidden) {

            if (textExists) {
                pGraphics->deleteText(textRef);
                textExists = false;
            }
        } else if (hidden != isHidden && !isHidden) {

            SplitGui::ResultValue<TextRef> textRes = pGraphics->drawText({extent.x, extent.y}, value, color, fontSize, depth);

            TRYD(textRes);

            textRef = textRes.value;

            textExists = true;
        }

        hidden = isHidden;

        return Result::eSuccess;
    }

    void Default::Text::cleanup() {
        SPLITGUI_PROFILE;

        if (textExists) {
            pGraphics->deleteText(textRef);
            textExists = false;
        }
    }

    void Default::Text::setSize(unsigned int size) {
        SPLITGUI_PROFILE;

        fontSize = size;
    }

    void Default::Text::setHorizontalAnchor(HorizontalAnchor anchor) {
        SPLITGUI_PROFILE;

        horizontalAnchor = anchor;
    }

    void Default::Text::setVerticalAnchor(VerticalAnchor anchor) {
        SPLITGUI_PROFILE;

        verticalAnchor = anchor;
    }
}