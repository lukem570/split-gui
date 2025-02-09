#ifndef SPLITGUI_UNIT_EXPRESSION_CPP
#define SPLITGUI_UNIT_EXPRESSION_CPP

#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include <cctype>
#include <vector>
#include <string>

namespace SplitGui {

    ResultValue<UnitExpressionToken> UnitExpressionEvaluator::nextToken(std::string& expression) {
        while (std::isspace(expression[index])) {
            if (expression.size() == ++index) {
                UnitExpressionToken token;
                token.type = UnitExpressionTokenType::eEndOfFile;
                return token;
            }
        }

        if (expression.size() == index) {
            UnitExpressionToken token;
            token.type = UnitExpressionTokenType::eEndOfFile;
            return token;
        }

        int beginIndex = index;

        if (std::isdigit(expression[index])) {
            while (std::isdigit(expression[index]) || expression[index] == '.') { index++; }
            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eLiteral;
            token.value = expression.substr(beginIndex, index - beginIndex);
            return token;
        }

        if (expression[index] == ',') {
            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eComma;
            token.value = ',';

            index++;

            return token;
        }

        if (expression[index] == '(') {
            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eBeginBracket;
            token.value = '(';

            index++;

            return token;
        }

        if (expression[index] == ')') {
            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eEndBracket;
            token.value = ')';

            index++;

            return token;
        }

        if (expression.substr(index, 2) == "px" || expression[index] == '%' || expression[index] == 'u') {
            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eUnit;
            token.value = expression[index] == '%' ? "%" : (expression[index] == 'u' ? "u" : "px");

            index += token.value.size();

            return token;
        }

        if (expression[index] == 'i' || expression[index] == 'v') {
            if (expression[index] == 'i') {
                index++;
            }

            if (expression.substr(index, 3) != "vec") {
                index = beginIndex;
                goto BreakFromVector;
            }

            index += 4;

            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eVector;
            token.value = expression.substr(beginIndex, index - beginIndex);
            return token;
            
        }
        BreakFromVector:

        if (expression[index] == '+' ||
            expression[index] == '-' ||
            expression[index] == '*' ||
            expression[index] == '/'   ) {

            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eBinaryOp;
            token.value = expression[index];

            index++;

            return token;
        }

        if (std::isalnum(expression[index])) {
            while (std::isalnum(expression[++index])) {}

            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eCall;
            token.value = expression.substr(beginIndex, index - beginIndex);

            return token;
        }

        return Result::eInvalidToken;
    }

    ResultValue<UnitExpression*> UnitExpressionEvaluator::parse(std::string expression) {

        ResultValue<UnitExpressionToken> token;

        UnitExpression* ret = nullptr;

        while (true) {

            token = nextToken(expression);
            
            TRYD(token);

            if (token.value.type == UnitExpressionTokenType::eEndOfFile) {
                break;
            }

            switch (token.value.type) {
                case UnitExpressionTokenType::eLiteral: {
                    
                    UnitExpression* newExpression = new(std::nothrow) UnitExpression(UnitExpression::Type::eLiteral);

                    if (!newExpression) {
                        return Result::eHeapAllocFailed;
                    }

                    newExpression->literal.value = std::atof(token.value.value.c_str());

                    token = nextToken(expression);
                    TRYD(token);

                    if (token.value.type != UnitExpressionTokenType::eUnit) {
                        return Result::eInvalidToken;
                    }

                    newExpression->literal.type  = token.value.value == "%" ? UnitExpression::UnitType::ePercent : (token.value.value == "u" ? UnitExpression::UnitType::eUnsigned : UnitExpression::UnitType::ePixel);

                    ret = newExpression;

                    break;
                }
                case UnitExpressionTokenType::eCall: {
                    UnitExpression* newExpression = new(std::nothrow) UnitExpression(UnitExpression::Type::eCall);

                    if (!newExpression) {
                        return Result::eHeapAllocFailed;
                    }

                    // TODO:

                    ret = newExpression;
                    break;
                }
                case UnitExpressionTokenType::eVector: {

                    UnitExpression* newExpression = new(std::nothrow) UnitExpression(UnitExpression::Type::eVector);

                    if (!newExpression) {
                        return Result::eHeapAllocFailed;
                    }

                    newExpression->vector.isIVec = token.value.value[0] == 'i';

                    unsigned int size = token.value.value[newExpression->vector.isIVec ? 4 : 3] - '0';

                    if (size > 4 || size <= 1) {
                        return Result::eInvalidToken;
                    }

                    token = nextToken(expression);
                    TRYD(token);

                    if (token.value.type != UnitExpressionTokenType::eBeginBracket) {
                        return Result::eInvalidToken;
                    }
                    
                    SplitGui::UnitExpression** values = (SplitGui::UnitExpression**)std::malloc(size * sizeof(SplitGui::UnitExpression*));

                    if (values == nullptr) {
                        return Result::eHeapAllocFailed;
                    }

                    for (unsigned int i = 0; i < size; i++) {

                        ResultValue<UnitExpression*> parseRet = parse(expression);
                        TRYD(parseRet);

                        values[i] = parseRet.value;
                    }
                    newExpression->vector.values = values;
                    newExpression->vector.size = size;

                    ret = newExpression;
                    break;
                }
                case UnitExpressionTokenType::eBinaryOp: {
                    
                    UnitExpression* newExpression = new(std::nothrow) UnitExpression(UnitExpression::Type::eBinaryOp);

                    if (!newExpression) {
                        return Result::eHeapAllocFailed;
                    }

                    newExpression->binary.left  = ret;
                    newExpression->binary.oper  = enumerateOperator(token.value.value[0]);

                    ResultValue<UnitExpression*> parseRet = parse(expression);
                    TRYD(parseRet);

                    newExpression->binary.right = parseRet.value;

                    ret = newExpression;
                    
                    break;
                }
                case UnitExpressionTokenType::eComma: {
                    expressionTree = ret;

                    if (ret == nullptr) {
                        return Result::eInvalidToken;
                    }

                    return ret;
                }
                case UnitExpressionTokenType::eEndBracket: {
                    expressionTree = ret;

                    if (ret == nullptr) {
                        return Result::eInvalidToken;
                    }

                    return ret;
                }
                default: return Result::eInvalidType;
            }
        }

        expressionTree = ret;

        if (ret == nullptr) {
            return Result::eInvalidToken;
        }

        return ret;
    }

    UnitExpressionValue UnitExpressionEvaluator::evaluateExpr(int maxSize, UnitExpression* expression) {

        SPLITGUI_LOG("Evaluate: %d", (int)expression->type);

        switch (expression->type) {
            case UnitExpression::Type::eBinaryOp: {

                switch (expression->binary.oper) {
                    case UnitExpression::BinaryOpType::eAdd:      return evaluateExpr(maxSize, expression->binary.left) + evaluateExpr(maxSize, expression->binary.right); break;
                    case UnitExpression::BinaryOpType::eSubtract: return evaluateExpr(maxSize, expression->binary.left) - evaluateExpr(maxSize, expression->binary.right); break;
                    case UnitExpression::BinaryOpType::eMultiply: return evaluateExpr(maxSize, expression->binary.left) * evaluateExpr(maxSize, expression->binary.right); break;
                    case UnitExpression::BinaryOpType::eDivide:   return evaluateExpr(maxSize, expression->binary.left) / evaluateExpr(maxSize, expression->binary.right); break;
                }

                break;
            }
            case UnitExpression::Type::eCall: {
                // TODO:
                break;
            }
            case UnitExpression::Type::eVector: {
                  
                switch (expression->vector.size) {
                    case 2:
                        if (expression->vector.isIVec) {
                            return IVec2{(int) evaluateExpr(maxSize, expression->vector.values[0]).number, (int) evaluateExpr(maxSize, expression->vector.values[1]).number};
                        } else {
                            return Vec2{(float) evaluateExpr(maxSize, expression->vector.values[0]).number, (float) evaluateExpr(maxSize, expression->vector.values[1]).number};
                        }
                        break;
                    case 3:
                        if (expression->vector.isIVec) {
                            return IVec3{(int) evaluateExpr(maxSize, expression->vector.values[0]).number, (int) evaluateExpr(maxSize, expression->vector.values[1]).number, (int) evaluateExpr(maxSize, expression->vector.values[2]).number};
                        } else {
                            return Vec3{(float) evaluateExpr(maxSize, expression->vector.values[0]).number, (float) evaluateExpr(maxSize, expression->vector.values[1]).number, (float) evaluateExpr(maxSize, expression->vector.values[2]).number};
                        }
                        break;
                    case 4:
                        if (expression->vector.isIVec) {
                            return IVec4{(int) evaluateExpr(maxSize, expression->vector.values[0]).number, (int) evaluateExpr(maxSize, expression->vector.values[1]).number, (int) evaluateExpr(maxSize, expression->vector.values[2]).number, (int) evaluateExpr(maxSize, expression->vector.values[3]).number};
                        } else {
                            return Vec4{(float) evaluateExpr(maxSize, expression->vector.values[0]).number, (float) evaluateExpr(maxSize, expression->vector.values[1]).number, (float) evaluateExpr(maxSize, expression->vector.values[2]).number, (float) evaluateExpr(maxSize, expression->vector.values[3]).number};
                        }
                        break;
                } 

                break;     
            }
            case UnitExpression::Type::eLiteral: {

                if (expression->literal.type == UnitExpression::UnitType::ePercent) {
                    return (int)((expression->literal.value / 100.0) * (double)maxSize);
                } else if (expression->literal.type == UnitExpression::UnitType::ePixel || expression->literal.type == UnitExpression::UnitType::eUnsigned) {
                    return (int)expression->literal.value;
                } 
            }
            default: break;
        }

        return 0;
    }

    UnitExpressionValue UnitExpressionEvaluator::evaluate(int maxSize) {
        return evaluateExpr(maxSize, expressionTree);
    }

    UnitExpressionEvaluator::~UnitExpressionEvaluator() {
        cleanup(expressionTree);
    }

    void UnitExpressionEvaluator::cleanup(UnitExpression* expression) {
        if (!expression) {
            return;
        }
        
        switch (expression->type) {
            case UnitExpression::Type::eVector:

                for (unsigned int i = 0; i < expression->vector.size; i++) {
                    cleanup(expression->vector.values[i]);
                }

                free(expression->vector.values);
                
                break;
            case UnitExpression::Type::eCall:

                // TODO:

                //for (unsigned int i = 0; i < expression->call.params.size(); i++) {
                    //cleanup(expression->call.params[i]);
                //}

                break;
            case UnitExpression::Type::eBinaryOp:

                cleanup(expression->binary.left);
                cleanup(expression->binary.right);

                break;
            default: break;
        }

        delete expression;
    }

#define UNIT_EXPRESSION_OPERATOR(sign)                                  \
                                                                        \
    switch (type) {                                                     \
        case Type::eNumber: return number + operand.number; break;      \
        case Type::eVector: {                                           \
                                                                        \
            switch (vector.size) {                                      \
                case 2:                                                 \
                    if (vector.isInt) {                                 \
                        return vector.ivec2 sign operand.vector.ivec2;  \
                    } else {                                            \
                        return vector.vec2 sign operand.vector.vec2;    \
                    }                                                   \
                    break;                                              \
                case 3:                                                 \
                    if (vector.isInt) {                                 \
                        return vector.ivec3 sign operand.vector.ivec3;  \
                    } else {                                            \
                        return vector.vec3 sign operand.vector.vec3;    \
                    }                                                   \
                    break;                                              \
                case 4:                                                 \
                    if (vector.isInt) {                                 \
                        return vector.ivec4 sign operand.vector.ivec4;  \
                    } else {                                            \
                        return vector.vec4 sign operand.vector.vec4;    \
                    }                                                   \
                    break;                                              \
                default: break;                                         \
            }                                                           \
                                                                        \
            break;                                                      \
        default: break;                                                 \
        }                                                               \
    }                                                                   \
                                                                        \
    return -1.0;

// END UNIT_EXPRESSION_OPERATOR


    UnitExpressionValue UnitExpressionValue::operator+(const UnitExpressionValue& operand) { UNIT_EXPRESSION_OPERATOR(+) }
    UnitExpressionValue UnitExpressionValue::operator-(const UnitExpressionValue& operand) { UNIT_EXPRESSION_OPERATOR(-) }
    UnitExpressionValue UnitExpressionValue::operator*(const UnitExpressionValue& operand) { UNIT_EXPRESSION_OPERATOR(*) }
    UnitExpressionValue UnitExpressionValue::operator/(const UnitExpressionValue& operand) { UNIT_EXPRESSION_OPERATOR(/) }
}

#endif