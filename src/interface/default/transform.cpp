#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {

    Result Default::Transform::setScale(std::string scaleIn) {
        SplitGui::ResultValue<SplitGui::UnitExpression*> parseRes = scale.parse(scaleIn);
        TRYD(parseRes);

        if (parseRes.value->type != SplitGui::UnitExpression::Type::eVector || 
            parseRes.value->vector.isIVec != true                           || 
            parseRes.value->vector.size != 2) {

            return Result::eInvalidExpression;
        }

        return Result::eSuccess;
    }

    Result Default::Transform::setPosition(std::string positionIn) {
        SplitGui::ResultValue<SplitGui::UnitExpression*> parseRes = position.parse(positionIn);
        TRYD(parseRes);

        if (parseRes.value->type != SplitGui::UnitExpression::Type::eVector || 
            parseRes.value->vector.isIVec != true                           || 
            parseRes.value->vector.size != 2) {

            return Result::eInvalidExpression;
        }

        return Result::eSuccess;
    }

    void Default::Transform::setOrigin(bool isRelativeIn) {
        isRelative = isRelativeIn;
    }

    void Default::Transform::update() {
        ResultValue<UnitExpressionValue> positionXEvaluateRes = position.evaluate(extent.width);
        ResultValue<UnitExpressionValue> positionYEvaluateRes = position.evaluate(extent.height);
        
        ResultValue<UnitExpressionValue> scaleXEvaluateRes = scale.evaluate(extent.width);
        ResultValue<UnitExpressionValue> scaleYEvaluateRes = scale.evaluate(extent.height);
        
        RectObj childExtent;

        childExtent.x      = positionXEvaluateRes.value.vector.ivec2.x;
        childExtent.y      = positionYEvaluateRes.value.vector.ivec2.y;
        childExtent.height = scaleYEvaluateRes.value.vector.ivec2.y;
        childExtent.width  = scaleXEvaluateRes.value.vector.ivec2.x;

        children.back()->setExtent(childExtent);
        children.back()->update();
    }

    Result Default::Transform::instance() {
        if (maxChildren != children.size()) {
            return Result::eInvalidNumberOfChildren;
        }
        
        ResultValue<UnitExpressionValue> positionXEvaluateRes = position.evaluate(extent.width);
        ResultValue<UnitExpressionValue> positionYEvaluateRes = position.evaluate(extent.height);
        
        ResultValue<UnitExpressionValue> scaleXEvaluateRes = scale.evaluate(extent.width);
        ResultValue<UnitExpressionValue> scaleYEvaluateRes = scale.evaluate(extent.height);
        
        TRYD(positionXEvaluateRes);
        TRYD(positionYEvaluateRes);
        TRYD(scaleXEvaluateRes);
        TRYD(scaleYEvaluateRes);
        
        RectObj childExtent;

        childExtent.x      = positionXEvaluateRes.value.vector.ivec2.x;
        childExtent.y      = positionYEvaluateRes.value.vector.ivec2.y;
        childExtent.height = scaleYEvaluateRes.value.vector.ivec2.y;
        childExtent.width  = scaleXEvaluateRes.value.vector.ivec2.x;

        children.back()->setGraphics(pGraphics);
        children.back()->setExtent(childExtent);
        children.back()->instance();

        SPLITGUI_LOG("Instanced Transform");

        return Result::eSuccess;
    }
}