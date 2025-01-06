#include <string>
#include <vector>
#include <cctype>
#include <sstream>

namespace SplitGui {

    enum class TokenType {
        eTagOpen,
        eTagClose,
        eText,
        eAttribute,
        eEndOfFile,
        eError
    };

    struct Token {
        TokenType type;
        std::string value;
    };

    
};