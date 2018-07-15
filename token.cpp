#include "token.h"

using namespace std;

string Token::toString() const {
    return names[type] + " " + lexeme + " (" + literal.toString() + \
        ") at " + std::to_string(line);
}

const string Token::names[] = {
    "LeftParen",
    "RightParen",
    "LeftBrace",
    "RightBrace",
    "Comma",
    "Dot",
    "Minus",
    "Plus",
    "Semicolon",
    "Slash",
    "Star",
    "Bang",
    "BangEqual",
    "Equal",
    "EqualEqual",
    "Greater",
    "GreaterEqual",
    "Less",
    "LessEqual",
    "Identifier",
    "String",
    "Number",
    "And",
    "Class",
    "Else",
    "False",
    "Fun",
    "For",
    "If",
    "Nil",
    "Or",
    "Print",
    "Return",
    "Super",
    "This",
    "True",
    "Var",
    "While",
    "Eof"
};
