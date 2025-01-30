#include <splitgui/structs.hpp>
#include <splitgui/interface.hpp>

namespace SplitGui {

    UnitExpressionToken getNextUnitExpressionToken(int& index, std::string& expression) {
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

    UnitExpression::BinaryOpType enumerateOperator(char oper) {
        switch (oper) {
            case '+': return UnitExpression::BinaryOpType::eAdd;      break;
            case '-': return UnitExpression::BinaryOpType::eSubtract; break;
            case '*': return UnitExpression::BinaryOpType::eMultiply; break;
            case '/': return UnitExpression::BinaryOpType::eDivide;   break;
            default: throw;
        }
    }

    UnitExpression* parseUnitExpression(std::string& expression, int index = 0) {

        UnitExpressionToken token;

        UnitExpression* result = nullptr;

        while ((token = getNextUnitExpressionToken(index, expression)).type != UnitExpressionTokenType::eEndOfFile) {
            switch (token.type) {
                case UnitExpressionTokenType::eLiteral: {
                    
                    UnitExpression* newExpression = new UnitExpression();

                    newExpression->type          = UnitExpression::Type::eLiteral;
                    newExpression->literal.value = std::atof(token.value.c_str());

                    token = getNextUnitExpressionToken(index, expression);

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
                    newExpression->binary.right = parseUnitExpression(expression, index);

                    result = newExpression;

                    goto ReturnUnitParse;
                    
                    break;
                }
                default: throw; // invalid type
            }
        }
        ReturnUnitParse:

        return result;
    }

    int generateValue(UnitExpression* expression, int maxSize) {
        switch (expression->type) {
            case UnitExpression::Type::eBinaryOp: {
                
                switch (expression->binary.oper) {
                    case UnitExpression::BinaryOpType::eAdd:      return generateValue(expression->binary.left, maxSize) + generateValue(expression->binary.right, maxSize); break;
                    case UnitExpression::BinaryOpType::eSubtract: return generateValue(expression->binary.left, maxSize) - generateValue(expression->binary.right, maxSize); break;
                    case UnitExpression::BinaryOpType::eMultiply: return generateValue(expression->binary.left, maxSize) * generateValue(expression->binary.right, maxSize); break;
                    case UnitExpression::BinaryOpType::eDivide:   return generateValue(expression->binary.left, maxSize) / generateValue(expression->binary.right, maxSize); break;
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

    void cleanupUnitExpression(UnitExpression* expression) {
        switch (expression->type) {
            case UnitExpression::Type::eVector:

                for (int i = 0; i < expression->vector.values.size(); i++) {
                    cleanupUnitExpression(expression->vector.values[i]);
                }
                
                break;
            case UnitExpression::Type::eCall:

                for (int i = 0; i < expression->call.params.size(); i++) {
                    cleanupUnitExpression(expression->call.params[i]);
                }

                break;
            case UnitExpression::Type::eBinaryOp:

                cleanupUnitExpression(expression->binary.left);
                cleanupUnitExpression(expression->binary.right);

                break;
        }

        delete expression;
    }

    int evaluateUnitExpression(std::string expression, int maxSize) {
        UnitExpression* expressionTree = parseUnitExpression(expression);
        int value = generateValue(expressionTree, maxSize);
        cleanupUnitExpression(expressionTree);

        printf("evaluated to: %d\n", value);

        return value;
    }
}