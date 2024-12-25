#include <string>
#include <vector>
#include <sstream>

namespace SplitGui {
    namespace XmlParser {
        enum class TokenType {
            eLeftBracket,
            eRightBracket,
            eName,
            eValue,
            eSlash,
            eEquals,
        };

        struct Token {
            std::string value;
            TokenType type;
        };

        struct Command {
            std::vector<Token> tokens;
        };

        std::vector<Command> tokenize(std::string& data) {
            std::vector<Command> commands;

            bool isNotName = false;
            bool insideBrackets = false;
            bool isValue = false;

            std::stringstream valueBuffer;

            for (int i = 0; i < data.size(); i++) {
                
                if (data[i] == ' ' || data[i] == '\t' || data[i] == '\n') {
                    continue;
                }

                if (!insideBrackets) {

                    if (data[i] == '>' || data[i] == '=') {
                        printf("ERROR: invalid token inside xml file at %d\n", i);
                        throw;
                    }


                    if (data[i] == '<') {

                        Token newToken;
                        newToken.type = TokenType::eLeftBracket;
                        newToken.value = '<';

                        commands.push_back({});
                        commands.back().tokens.push_back(newToken);

                        insideBrackets = true;

                    } else {
                        while (data[i] != '<') {
                            valueBuffer << data[i];
                            i++;
                        }

                        Token newToken;
                        newToken.type = TokenType::eValue;
                        newToken.value = valueBuffer.str();

                        valueBuffer.str("");

                        commands.push_back({});
                        commands.back().tokens.push_back(newToken);
                    }

                } else {

                    if (data[i] == '/') {
                        Token newToken;
                        newToken.type = TokenType::eSlash;
                        newToken.value = '/';

                        commands.back().tokens.push_back(newToken);

                        continue;
                    }

                    if (data[i] == '=') {
                        Token newToken;
                        newToken.type = TokenType::eSlash;
                        newToken.value = '/';

                        commands.back().tokens.push_back(newToken);

                        continue;
                    }

                    if (data[i] == '>') {
                        Token newToken;
                        newToken.type = TokenType::eRightBracket;
                        newToken.value = '>';

                        commands.back().tokens.push_back(newToken);

                        isNotName = false;
                        insideBrackets = false;
                        isValue = false;

                        continue;
                    }


                    if (!isNotName) {
                        while (data[i] != ' ' && data[i] != '\t' && data[i] != '\n') {
                            valueBuffer << data[i];
                            i++;
                        }

                        Token newToken;
                        newToken.type = TokenType::eName;
                        newToken.value = valueBuffer.str();

                        valueBuffer.str("");

                        commands.back().tokens.push_back(newToken);

                        isNotName = true;
                    }
                    
                    while (data[i] != ' ' && data[i] != '\t' && data[i] != '\n' && data[i] != '=') {
                        valueBuffer << data[i];
                        i++;
                    }

                    Token newToken;
                    newToken.type = TokenType::eValue;
                    newToken.value = valueBuffer.str();

                    valueBuffer.str("");

                    commands.back().tokens.push_back(newToken);
                }
            }

            return commands;
            
        }

    };
};