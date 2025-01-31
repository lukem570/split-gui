#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include <cctype>

namespace SplitGui {

    UnitExpressionToken UnitExpressionEvaluator::nextToken(std::string& expression) {
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

        if (expression.substr(index, 2) == "px" || expression[index] == '%') {
            UnitExpressionToken token;
            token.type  = UnitExpressionTokenType::eUnit;
            token.value = expression[index] == '%' ? "%" : "px";

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

        throw; // invalid char
    }

    UnitExpression* UnitExpressionEvaluator::parse(std::string expression) {

        UnitExpressionToken token;

        UnitExpression* result = nullptr;

        while ((token = nextToken(expression)).type != UnitExpressionTokenType::eEndOfFile) {
            switch (token.type) {
                case UnitExpressionTokenType::eLiteral: {
                    
                    UnitExpression* newExpression = new UnitExpression();

                    newExpression->type          = UnitExpression::Type::eLiteral;
                    newExpression->literal.value = std::atof(token.value.c_str());

                    token = nextToken(expression);

                    if (token.type != UnitExpressionTokenType::eUnit) {
                        throw;
                    }

                    newExpression->literal.type  = token.value == "%" ? UnitExpression::UnitType::ePercent : UnitExpression::UnitType::ePixel;

                    result = newExpression;

                    break;
                }
                case UnitExpressionTokenType::eBinaryOp: {
                    
                    UnitExpression* newExpression = new UnitExpression();

                    newExpression->type         = UnitExpression::Type::eBinaryOp;
                    newExpression->binary.left  = result;
                    newExpression->binary.oper  = enumerateOperator(token.value[0]);
                    newExpression->binary.right = parse(expression);

                    result = newExpression;

                    
                    break;
                }
                default: throw; // invalid type
            }
        }

        expressionTree = result;

        return result;
    }

    int UnitExpressionEvaluator::evaluateExpr(int maxSize, UnitExpression* expression) {

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
            case UnitExpression::Type::eLiteral: {

                if (expression->literal.type == UnitExpression::UnitType::ePercent) {
                    return (int)((expression->literal.value / 100.0) * (double)maxSize);
                } else if (expression->literal.type == UnitExpression::UnitType::ePixel) {
                    return (int)expression->literal.value;
                }
            }
            default:
                break;
        }

        return 0;
    }

    int UnitExpressionEvaluator::evaluate(int maxSize) {
        return evaluateExpr(maxSize, expressionTree);
    }

    UnitExpressionEvaluator::~UnitExpressionEvaluator() {
        cleanup(expressionTree);
    }

    void UnitExpressionEvaluator::cleanup(UnitExpression* expression) {
        switch (expression->type) {
            case UnitExpression::Type::eVector:

                for (int i = 0; i < expression->vector.values.size(); i++) {
                    cleanup(expression->vector.values[i]);
                }
                
                break;
            case UnitExpression::Type::eCall:

                for (int i = 0; i < expression->call.params.size(); i++) {
                    cleanup(expression->call.params[i]);
                }

                break;
            case UnitExpression::Type::eBinaryOp:

                cleanup(expression->binary.left);
                cleanup(expression->binary.right);

                break;
        }

        delete expression;
    }
}