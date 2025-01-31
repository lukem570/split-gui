#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

#include <cctype>

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

        return Result::eInvalidToken;
    }

    ResultValue<UnitExpression*> UnitExpressionEvaluator::parse(std::string expression) {

        ResultValue<UnitExpressionToken> token;

        UnitExpression* ret = nullptr;

        while (true) {

            token = nextToken(expression);

            TRY(token);

            if (token.value.type == UnitExpressionTokenType::eEndOfFile) {
                break;
            }

            switch (token.value.type) {
                case UnitExpressionTokenType::eLiteral: {
                    
                    UnitExpression* newExpression = new UnitExpression();

                    newExpression->type          = UnitExpression::Type::eLiteral;
                    newExpression->literal.value = std::atof(token.value.value.c_str());

                    token = nextToken(expression);

                    TRY(token);

                    if (token.value.type != UnitExpressionTokenType::eUnit) {
                        return Result::eInvalidToken;
                    }

                    newExpression->literal.type  = token.value.value == "%" ? UnitExpression::UnitType::ePercent : UnitExpression::UnitType::ePixel;

                    ret = newExpression;

                    break;
                }
                case UnitExpressionTokenType::eBinaryOp: {
                    
                    UnitExpression* newExpression = new UnitExpression();

                    newExpression->type         = UnitExpression::Type::eBinaryOp;
                    newExpression->binary.left  = ret;
                    newExpression->binary.oper  = enumerateOperator(token.value.value[0]);

                    ResultValue<UnitExpression*> parseRet = parse(expression);

                    TRY(parseRet);

                    newExpression->binary.right = parseRet.value;

                    ret = newExpression;
                    
                    break;
                }
                default: return Result::eInvalidType;
            }
        }

        expressionTree = ret;

        return ret;
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